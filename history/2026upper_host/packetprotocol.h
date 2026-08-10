#pragma once

#include <QByteArray>
#include <QtGlobal>

namespace HostProtocol {

constexpr quint32 kMagic = 0x32505345; // "ESP2" little-endian
constexpr quint16 kVersion = 1;
constexpr int kHeaderSize = 40;
constexpr int kMaxPayload = 8 * 1024 * 1024;

constexpr quint16 kTypeImageRgb888 = 0x01;
constexpr quint16 kTypeMetricsJson = 0x02;
constexpr quint16 kTypeDetectionJson = 0x03;
constexpr quint16 kTypeTimeSyncJson = 0x10;
constexpr quint16 kTypeControlJson = 0x11;
constexpr quint16 kTypeInferenceResultJson = 0x12;

constexpr quint16 kPixelRgb888 = 1;
constexpr quint16 kPixelJpeg = 2;

struct PacketHeader {
    quint16 type = 0;
    quint32 seq = 0;
    quint64 timestampMs = 0;
    quint32 payloadLen = 0;
    quint16 width = 0;
    quint16 height = 0;
    quint16 pixelFormat = 0;
};

bool parseHeader(const QByteArray &buffer, PacketHeader *header);
QByteArray makeJsonPacket(quint16 type, quint32 seq, const QByteArray &json);

}
