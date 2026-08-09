#include "packetprotocol.h"

#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtEndian>

namespace {
void writeLe16(QByteArray *out, quint16 value)
{
    const int old = out->size();
    out->resize(old + 2);
    qToLittleEndian(value, out->data() + old);
}

void writeLe32(QByteArray *out, quint32 value)
{
    const int old = out->size();
    out->resize(old + 4);
    qToLittleEndian(value, out->data() + old);
}

void writeLe64(QByteArray *out, quint64 value)
{
    const int old = out->size();
    out->resize(old + 8);
    qToLittleEndian(value, out->data() + old);
}
}

namespace HostProtocol {

bool parseHeader(const QByteArray &buffer, PacketHeader *header)
{
    if (buffer.size() < kHeaderSize || !header) {
        return false;
    }

    const uchar *p = reinterpret_cast<const uchar *>(buffer.constData());
    const quint32 magic = qFromLittleEndian<quint32>(p);
    const quint16 version = qFromLittleEndian<quint16>(p + 4);
    const quint16 type = qFromLittleEndian<quint16>(p + 6);
    const quint32 headerSize = qFromLittleEndian<quint32>(p + 8);
    const bool versionValid = type == kTypeImageResult
        ? version == kVersionV1 || version == kImageVersionV2
        : version == kVersionV1;
    if (magic != kMagic || !versionValid || headerSize != kHeaderSize) {
        return false;
    }

    header->version = version;
    header->type = type;
    header->seq = qFromLittleEndian<quint32>(p + 12);
    header->timestampMs = qFromLittleEndian<quint64>(p + 16);
    header->payloadLen = qFromLittleEndian<quint32>(p + 24);
    header->width = qFromLittleEndian<quint16>(p + 28);
    header->height = qFromLittleEndian<quint16>(p + 30);
    header->pixelFormat = qFromLittleEndian<quint16>(p + 32);
    header->reserved = qFromLittleEndian<quint16>(p + 34);
    header->reserved2 = qFromLittleEndian<quint32>(p + 36);
    return true;
}

bool parseImageResultV2(const PacketHeader &header, const QByteArray &payload,
                        ImageResultV2 *result, QString *error)
{
    auto fail = [error](const QString &message) {
        if (error) {
            *error = message;
        }
        return false;
    };
    if (!result || header.version != kImageVersionV2 || header.type != kTypeImageResult ||
        header.pixelFormat != kPixelJpeg || header.width == 0 || header.height == 0) {
        return fail(QStringLiteral("V2 图像公共头无效"));
    }
    if (payload.size() < kImageMetaV2Size) {
        return fail(QStringLiteral("V2 图像元数据不完整"));
    }

    const uchar *p = reinterpret_cast<const uchar *>(payload.constData());
    const quint32 frameId = qFromLittleEndian<quint32>(p);
    const quint64 captureTimestampUs = qFromLittleEndian<quint64>(p + 4);
    const quint32 jpegLen = qFromLittleEndian<quint32>(p + 12);
    const quint16 boxCount = qFromLittleEndian<quint16>(p + 16);
    const quint16 primaryClassId = qFromLittleEndian<quint16>(p + 18);
    const quint16 primaryConfidence = qFromLittleEndian<quint16>(p + 20);
    const quint16 inferTimeMs = qFromLittleEndian<quint16>(p + 22);
    const quint16 sourceWidth = qFromLittleEndian<quint16>(p + 24);
    const quint16 sourceHeight = qFromLittleEndian<quint16>(p + 26);
    const quint16 metaSize = qFromLittleEndian<quint16>(p + 28);
    const quint16 boxSize = qFromLittleEndian<quint16>(p + 30);

    if (metaSize != kImageMetaV2Size || boxSize != kImageBoxV2Size || boxCount > kImageMaxBoxes) {
        return fail(QStringLiteral("V2 图像结构尺寸无效"));
    }
    if (sourceWidth != header.width || sourceHeight != header.height) {
        return fail(QStringLiteral("V2 图像尺寸不一致"));
    }
    const quint64 expected = quint64(kImageMetaV2Size) + quint64(boxCount) * kImageBoxV2Size + jpegLen;
    if (expected != quint64(payload.size())) {
        return fail(QStringLiteral("V2 图像负载长度不一致"));
    }

    QList<ImageBoxV2> boxes;
    boxes.reserve(boxCount);
    const uchar *boxData = p + kImageMetaV2Size;
    for (quint16 i = 0; i < boxCount; ++i) {
        const uchar *b = boxData + i * kImageBoxV2Size;
        ImageBoxV2 box;
        box.stage = b[0];
        box.category = b[1];
        box.scoreX1000 = qFromLittleEndian<quint16>(b + 2);
        box.x1 = qFromLittleEndian<quint16>(b + 4);
        box.y1 = qFromLittleEndian<quint16>(b + 6);
        box.x2 = qFromLittleEndian<quint16>(b + 8);
        box.y2 = qFromLittleEndian<quint16>(b + 10);
        box.flags = qFromLittleEndian<quint16>(b + 12);
        if (box.scoreX1000 > 1000 || box.x1 > box.x2 || box.y1 > box.y2 ||
            box.x2 >= sourceWidth || box.y2 >= sourceHeight) {
            return fail(QStringLiteral("V2 图像检测框无效"));
        }
        boxes.append(box);
    }

    const QByteArray jpeg = payload.mid(kImageMetaV2Size + boxCount * kImageBoxV2Size, jpegLen);
    if (jpeg.size() != int(jpegLen) || jpeg.size() < 2 || !jpeg.startsWith("\xff\xd8")) {
        return fail(QStringLiteral("V2 JPEG 数据无效"));
    }

    result->frameId = frameId;
    result->captureTimestampUs = captureTimestampUs;
    result->width = sourceWidth;
    result->height = sourceHeight;
    result->primaryClassId = primaryClassId;
    result->primaryConfidenceX1000 = qMin<quint16>(1000, primaryConfidence);
    result->inferTimeMs = inferTimeMs;
    result->boxes = boxes;
    result->jpeg = jpeg;
    return true;
}

bool normalizeImageBox(quint16 imageWidth, quint16 imageHeight, const ImageBoxV2 &box,
                       NormalizedImageBox *normalized)
{
    if (!normalized || imageWidth == 0 || imageHeight == 0 || box.x1 > box.x2 || box.y1 > box.y2 ||
        box.x2 >= imageWidth || box.y2 >= imageHeight) {
        return false;
    }
    normalized->x = double(box.x1) / imageWidth;
    normalized->y = double(box.y1) / imageHeight;
    normalized->width = double(box.x2 - box.x1 + 1) / imageWidth;
    normalized->height = double(box.y2 - box.y1 + 1) / imageHeight;
    return true;
}

QByteArray makeJsonPacket(quint16 type, quint32 seq, const QByteArray &json)
{
    QByteArray packet;
    packet.reserve(kHeaderSize + json.size());
    writeLe32(&packet, kMagic);
    writeLe16(&packet, kVersionV1);
    writeLe16(&packet, type);
    writeLe32(&packet, kHeaderSize);
    writeLe32(&packet, seq);
    writeLe64(&packet, quint64(QDateTime::currentMSecsSinceEpoch()));
    writeLe32(&packet, quint32(json.size()));
    writeLe16(&packet, 0);
    writeLe16(&packet, 0);
    writeLe16(&packet, 0);
    writeLe16(&packet, 0);
    writeLe32(&packet, 0);
    packet.append(json);
    return packet;
}

QByteArray makeControlGetJson()
{
    QJsonObject request;
    request.insert(QStringLiteral("op"), QStringLiteral("get"));
    return QJsonDocument(request).toJson(QJsonDocument::Compact);
}

QByteArray makeControlSetJson(const QString &key, const QJsonValue &value)
{
    QJsonObject request;
    request.insert(QStringLiteral("op"), QStringLiteral("set"));
    request.insert(QStringLiteral("key"), key);
    request.insert(QStringLiteral("value"), value);
    return QJsonDocument(request).toJson(QJsonDocument::Compact);
}

QByteArray makeControlActionJson(const QString &key)
{
    QJsonObject request;
    request.insert(QStringLiteral("op"), QStringLiteral("action"));
    request.insert(QStringLiteral("key"), key);
    return QJsonDocument(request).toJson(QJsonDocument::Compact);
}

bool parseControlJson(const QByteArray &payload, QJsonObject *message, QString *error)
{
    auto fail = [error](const QString &text) {
        if (error) {
            *error = text;
        }
        return false;
    };
    if (!message) {
        return fail(QStringLiteral("控制消息输出对象为空"));
    }

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(payload, &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        return fail(QStringLiteral("控制 JSON 无效"));
    }

    const QJsonObject object = document.object();
    const QString op = object.value(QStringLiteral("op")).toString();
    if (op != QStringLiteral("state") && op != QStringLiteral("error")) {
        return fail(QStringLiteral("控制消息 op 无效"));
    }
    if (op == QStringLiteral("state") && !object.value(QStringLiteral("settings")).isObject()) {
        return fail(QStringLiteral("控制状态缺少 settings"));
    }
    if (op == QStringLiteral("error") && !object.value(QStringLiteral("message")).isString()) {
        return fail(QStringLiteral("控制错误缺少 message"));
    }

    *message = object;
    return true;
}

}
