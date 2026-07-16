#pragma once

#include <QByteArray>
#include <QList>
#include <QString>
#include <QtGlobal>

namespace HostProtocol {

constexpr quint32 kMagic = 0x32505345; // "ESP2" little-endian
constexpr quint16 kVersionV1 = 1;
constexpr quint16 kImageVersionV2 = 2;
constexpr int kHeaderSize = 40;
constexpr int kImageMetaV2Size = 32;
constexpr int kImageBoxV2Size = 16;
constexpr int kImageMaxBoxes = 8;
constexpr int kMaxPayload = 8 * 1024 * 1024;

constexpr quint16 kTypeImageResult = 0x01;
constexpr quint16 kTypeMetricsJson = 0x02;
constexpr quint16 kTypeDetectionJson = 0x03;
constexpr quint16 kTypeTimeSyncJson = 0x10;
constexpr quint16 kTypeControlJson = 0x11;
constexpr quint16 kTypeSimLine = 0x12;

constexpr quint16 kPixelRgb888 = 1;
constexpr quint16 kPixelJpeg = 2;

struct PacketHeader {
    quint16 version = 0;
    quint16 type = 0;
    quint32 seq = 0;
    quint64 timestampMs = 0;
    quint32 payloadLen = 0;
    quint16 width = 0;
    quint16 height = 0;
    quint16 pixelFormat = 0;
    quint16 reserved = 0;
    quint32 reserved2 = 0;
};

struct ImageBoxV2 {
    quint8 stage = 0;
    quint8 category = 0;
    quint16 scoreX1000 = 0;
    quint16 x1 = 0;
    quint16 y1 = 0;
    quint16 x2 = 0;
    quint16 y2 = 0;
    quint16 flags = 0;
};

struct ImageResultV2 {
    quint32 frameId = 0;
    quint64 captureTimestampUs = 0;
    quint16 width = 0;
    quint16 height = 0;
    quint16 primaryClassId = 0;
    quint16 primaryConfidenceX1000 = 0;
    quint16 inferTimeMs = 0;
    QList<ImageBoxV2> boxes;
    QByteArray jpeg;
};

bool parseHeader(const QByteArray &buffer, PacketHeader *header);
bool parseImageResultV2(const PacketHeader &header, const QByteArray &payload,
                        ImageResultV2 *result, QString *error);
QByteArray makeJsonPacket(quint16 type, quint32 seq, const QByteArray &json);

}
