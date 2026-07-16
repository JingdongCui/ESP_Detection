#include "../packetprotocol.h"

#include <QTest>
#include <QtEndian>

namespace {

void putLe16(QByteArray *bytes, int offset, quint16 value)
{
    qToLittleEndian(value, bytes->data() + offset);
}

void putLe32(QByteArray *bytes, int offset, quint32 value)
{
    qToLittleEndian(value, bytes->data() + offset);
}

void putLe64(QByteArray *bytes, int offset, quint64 value)
{
    qToLittleEndian(value, bytes->data() + offset);
}

QByteArray makeHeader(quint16 version, quint16 type, quint32 payloadLen,
                      quint16 width = 640, quint16 height = 375,
                      quint16 pixelFormat = HostProtocol::kPixelJpeg)
{
    QByteArray header(HostProtocol::kHeaderSize, '\0');
    putLe32(&header, 0, HostProtocol::kMagic);
    putLe16(&header, 4, version);
    putLe16(&header, 6, type);
    putLe32(&header, 8, HostProtocol::kHeaderSize);
    putLe32(&header, 12, 17);
    putLe64(&header, 16, 123456);
    putLe32(&header, 24, payloadLen);
    putLe16(&header, 28, width);
    putLe16(&header, 30, height);
    putLe16(&header, 32, pixelFormat);
    return header;
}

QByteArray makeV2Payload()
{
    const QByteArray jpeg = QByteArray::fromHex("ffd8ffd9");
    QByteArray payload(HostProtocol::kImageMetaV2Size + HostProtocol::kImageBoxV2Size, '\0');
    putLe32(&payload, 0, 42);
    putLe64(&payload, 4, 987654321);
    putLe32(&payload, 12, jpeg.size());
    putLe16(&payload, 16, 1);
    putLe16(&payload, 18, 2);
    putLe16(&payload, 20, 875);
    putLe16(&payload, 22, 37);
    putLe16(&payload, 24, 640);
    putLe16(&payload, 26, 375);
    putLe16(&payload, 28, HostProtocol::kImageMetaV2Size);
    putLe16(&payload, 30, HostProtocol::kImageBoxV2Size);

    const int box = HostProtocol::kImageMetaV2Size;
    payload[box] = 1;
    payload[box + 1] = 2;
    putLe16(&payload, box + 2, 901);
    putLe16(&payload, box + 4, 64);
    putLe16(&payload, box + 6, 37);
    putLe16(&payload, box + 8, 319);
    putLe16(&payload, box + 10, 186);
    putLe16(&payload, box + 12, 3);
    payload.append(jpeg);
    return payload;
}

}

class PacketProtocolTests : public QObject
{
    Q_OBJECT

private slots:
    void acceptsV1AndV2ImageHeaders();
    void rejectsUnsupportedVersions();
    void parsesV2ImageResult();
    void rejectsMalformedV2Payloads();
    void normalizesInclusiveCoordinates();
};

void PacketProtocolTests::acceptsV1AndV2ImageHeaders()
{
    HostProtocol::PacketHeader parsed;
    QVERIFY(HostProtocol::parseHeader(makeHeader(HostProtocol::kVersionV1,
                                                  HostProtocol::kTypeImageResult, 4), &parsed));
    QCOMPARE(parsed.version, HostProtocol::kVersionV1);
    QCOMPARE(parsed.type, HostProtocol::kTypeImageResult);

    QVERIFY(HostProtocol::parseHeader(makeHeader(HostProtocol::kImageVersionV2,
                                                  HostProtocol::kTypeImageResult, 52), &parsed));
    QCOMPARE(parsed.version, HostProtocol::kImageVersionV2);
}

void PacketProtocolTests::rejectsUnsupportedVersions()
{
    HostProtocol::PacketHeader parsed;
    QVERIFY(!HostProtocol::parseHeader(makeHeader(HostProtocol::kImageVersionV2,
                                                   HostProtocol::kTypeMetricsJson, 0), &parsed));
    QVERIFY(!HostProtocol::parseHeader(makeHeader(3, HostProtocol::kTypeImageResult, 0), &parsed));
    QByteArray shortHeader(HostProtocol::kHeaderSize - 1, '\0');
    QVERIFY(!HostProtocol::parseHeader(shortHeader, &parsed));
}

void PacketProtocolTests::parsesV2ImageResult()
{
    const QByteArray payload = makeV2Payload();
    HostProtocol::PacketHeader header;
    QVERIFY(HostProtocol::parseHeader(makeHeader(HostProtocol::kImageVersionV2,
                                                  HostProtocol::kTypeImageResult, payload.size()), &header));

    HostProtocol::ImageResultV2 result;
    QString error;
    QVERIFY2(HostProtocol::parseImageResultV2(header, payload, &result, &error), qPrintable(error));
    QCOMPARE(result.frameId, quint32(42));
    QCOMPARE(result.captureTimestampUs, quint64(987654321));
    QCOMPARE(result.width, quint16(640));
    QCOMPARE(result.height, quint16(375));
    QCOMPARE(result.primaryClassId, quint16(2));
    QCOMPARE(result.primaryConfidenceX1000, quint16(875));
    QCOMPARE(result.inferTimeMs, quint16(37));
    QCOMPARE(result.boxes.size(), 1);
    QCOMPARE(result.boxes.first().stage, quint8(1));
    QCOMPARE(result.boxes.first().category, quint8(2));
    QCOMPARE(result.boxes.first().scoreX1000, quint16(901));
    QCOMPARE(result.jpeg, QByteArray::fromHex("ffd8ffd9"));
}

void PacketProtocolTests::rejectsMalformedV2Payloads()
{
    const QByteArray valid = makeV2Payload();
    HostProtocol::PacketHeader header;
    QVERIFY(HostProtocol::parseHeader(makeHeader(HostProtocol::kImageVersionV2,
                                                  HostProtocol::kTypeImageResult, valid.size()), &header));

    HostProtocol::ImageResultV2 result;
    QVERIFY(!HostProtocol::parseImageResultV2(header, valid.left(20), &result));

    QByteArray badCount = valid;
    putLe16(&badCount, 16, HostProtocol::kImageMaxBoxes + 1);
    QVERIFY(!HostProtocol::parseImageResultV2(header, badCount, &result));

    QByteArray badCoordinate = valid;
    putLe16(&badCoordinate, HostProtocol::kImageMetaV2Size + 8, 640);
    QVERIFY(!HostProtocol::parseImageResultV2(header, badCoordinate, &result));

    QByteArray badLength = valid;
    putLe32(&badLength, 12, 99);
    QVERIFY(!HostProtocol::parseImageResultV2(header, badLength, &result));

    QByteArray badJpeg = valid;
    badJpeg[HostProtocol::kImageMetaV2Size + HostProtocol::kImageBoxV2Size] = '\0';
    QVERIFY(!HostProtocol::parseImageResultV2(header, badJpeg, &result));
}

void PacketProtocolTests::normalizesInclusiveCoordinates()
{
    HostProtocol::ImageBoxV2 fullFrame;
    fullFrame.x2 = 639;
    fullFrame.y2 = 374;
    HostProtocol::NormalizedImageBox normalized;
    QVERIFY(HostProtocol::normalizeImageBox(640, 375, fullFrame, &normalized));
    QCOMPARE(normalized.x, 0.0);
    QCOMPARE(normalized.y, 0.0);
    QCOMPARE(normalized.width, 1.0);
    QCOMPARE(normalized.height, 1.0);

    HostProtocol::ImageBoxV2 box;
    box.x1 = 64;
    box.y1 = 37;
    box.x2 = 319;
    box.y2 = 186;
    QVERIFY(HostProtocol::normalizeImageBox(640, 375, box, &normalized));
    QCOMPARE(normalized.x, 0.1);
    QCOMPARE(normalized.width, 0.4);
    QCOMPARE(normalized.height, 0.4);

    box.x2 = 640;
    QVERIFY(!HostProtocol::normalizeImageBox(640, 375, box, &normalized));
}

QTEST_APPLESS_MAIN(PacketProtocolTests)
#include "packetprotocol_tests.moc"
