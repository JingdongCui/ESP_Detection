#include "packetprotocol.h"

#include <QDateTime>
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
    const quint32 headerSize = qFromLittleEndian<quint32>(p + 8);
    if (magic != kMagic || version != kVersion || headerSize != kHeaderSize) {
        return false;
    }

    header->type = qFromLittleEndian<quint16>(p + 6);
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

QByteArray makeJsonPacket(quint16 type, quint32 seq, const QByteArray &json)
{
    QByteArray packet;
    packet.reserve(kHeaderSize + json.size());
    writeLe32(&packet, kMagic);
    writeLe16(&packet, kVersion);
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

}
