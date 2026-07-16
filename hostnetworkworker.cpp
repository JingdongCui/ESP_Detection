#include "hostnetworkworker.h"

#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

namespace {
constexpr int kControlPort = 5000;
constexpr int kImagePort = 5001;
constexpr int kImageReadBufferBytes = 4 * 1024 * 1024;
constexpr int kSocketRecvBufferBytes = 1024 * 1024;

void setNativeReceiveBuffer(QTcpSocket *socket, int bytes)
{
    if (!socket) {
        return;
    }
    socket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, bytes);
}
}

HostNetworkWorker::HostNetworkWorker(QObject *parent)
    : QObject(parent)
{
    // Parent the servers to this worker so they follow it across moveToThread().
    m_controlServer.setParent(this);
    m_imageServer.setParent(this);
    connect(&m_controlServer, &QTcpServer::newConnection, this, &HostNetworkWorker::acceptControlConnection);
    connect(&m_imageServer, &QTcpServer::newConnection, this, &HostNetworkWorker::acceptImageConnection);
}

void HostNetworkWorker::start()
{
    listen();
}

void HostNetworkWorker::stop()
{
    if (m_control.socket) {
        m_control.socket->disconnectFromHost();
    }
    if (m_image.socket) {
        m_image.socket->disconnectFromHost();
    }
    m_controlServer.close();
    m_imageServer.close();
    emitState(QStringLiteral("未监听"));
}

void HostNetworkWorker::listen()
{
    if (m_controlServer.isListening() && m_imageServer.isListening()) {
        return;
    }

    const QHostAddress bindAddress(QStringLiteral("192.168.10.1"));
    bool controlOk = m_controlServer.listen(bindAddress, kControlPort);
    if (!controlOk) {
        controlOk = m_controlServer.listen(QHostAddress::AnyIPv4, kControlPort);
    }
    bool imageOk = m_imageServer.listen(bindAddress, kImagePort);
    if (!imageOk) {
        imageOk = m_imageServer.listen(QHostAddress::AnyIPv4, kImagePort);
    }

    if (!controlOk || !imageOk) {
        emitState(QStringLiteral("监听失败：control=%1 image=%2")
                      .arg(m_controlServer.errorString(), m_imageServer.errorString()));
        return;
    }

    emitState(QStringLiteral("正在监听 192.168.10.1:%1/%2").arg(kControlPort).arg(kImagePort));
}

void HostNetworkWorker::acceptControlConnection()
{
    while (m_controlServer.hasPendingConnections()) {
        QTcpSocket *socket = m_controlServer.nextPendingConnection();
        if (m_control.socket) {
            m_control.socket->disconnect(this);
            m_control.socket->close();
            m_control.socket->deleteLater();
        }
        m_control.socket = socket;
        m_control.buffer.clear();
        configureSocket(socket, false);
        connect(socket, &QTcpSocket::readyRead, this, [this]() { readFrom(&m_control, false); });
        connect(socket, &QTcpSocket::disconnected, this, [this, socket]() { onSocketDisconnected(socket, false); });
        emitState(QStringLiteral("控制已连接：%1:%2").arg(socket->peerAddress().toString()).arg(socket->peerPort()));
        sendInitialControls();
    }
}

void HostNetworkWorker::acceptImageConnection()
{
    while (m_imageServer.hasPendingConnections()) {
        QTcpSocket *socket = m_imageServer.nextPendingConnection();
        if (m_image.socket) {
            m_image.socket->disconnect(this);
            m_image.socket->close();
            m_image.socket->deleteLater();
        }
        m_image.socket = socket;
        m_image.buffer.clear();
        configureSocket(socket, true);
        connect(socket, &QTcpSocket::readyRead, this, [this]() { readFrom(&m_image, true); });
        connect(socket, &QTcpSocket::disconnected, this, [this, socket]() { onSocketDisconnected(socket, true); });
        emitState(QStringLiteral("图像已连接：%1:%2").arg(socket->peerAddress().toString()).arg(socket->peerPort()));
    }
}

void HostNetworkWorker::configureSocket(QTcpSocket *socket, bool imageSocket)
{
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    if (imageSocket) {
        socket->setReadBufferSize(kImageReadBufferBytes);
        setNativeReceiveBuffer(socket, kSocketRecvBufferBytes);
    }
}

void HostNetworkWorker::onSocketDisconnected(QTcpSocket *socket, bool imageSocket)
{
    if (imageSocket && m_image.socket == socket) {
        m_image.socket->deleteLater();
        m_image.socket = nullptr;
        m_image.buffer.clear();
    } else if (!imageSocket && m_control.socket == socket) {
        m_control.socket->deleteLater();
        m_control.socket = nullptr;
        m_control.buffer.clear();
    }
    emitState(QStringLiteral("下位机连接断开"));
}

void HostNetworkWorker::readFrom(StreamState *state, bool imageStream)
{
    if (!state || !state->socket) {
        return;
    }
    const QByteArray data = state->socket->readAll();
    if (data.isEmpty()) {
        return;
    }
    emit bytesReceived(data.size());
    state->buffer.append(data);
    processBuffer(state, imageStream);
}

void HostNetworkWorker::processBuffer(StreamState *state, bool imageStream)
{
    while (state->buffer.size() >= HostProtocol::kHeaderSize) {
        HostProtocol::PacketHeader header;
        if (!HostProtocol::parseHeader(state->buffer, &header)) {
            state->buffer.remove(0, 1);
            continue;
        }
        if (header.payloadLen > HostProtocol::kMaxPayload) {
            emit logLineReady(QStringLiteral("丢弃异常负载长度 %1").arg(header.payloadLen));
            state->buffer.remove(0, HostProtocol::kHeaderSize);
            continue;
        }
        const qsizetype packetSize = HostProtocol::kHeaderSize + qsizetype(header.payloadLen);
        if (state->buffer.size() < packetSize) {
            return;
        }
        const QByteArray payload = state->buffer.mid(HostProtocol::kHeaderSize, header.payloadLen);
        state->buffer.remove(0, packetSize);
        handlePacket(header, payload, imageStream);
    }
}

void HostNetworkWorker::handlePacket(const HostProtocol::PacketHeader &header, const QByteArray &payload, bool imageStream)
{
    if (header.type == HostProtocol::kTypeImageResult) {
        if (!imageStream) {
            emit logLineReady(QStringLiteral("图像事务出现在控制通道 seq=%1").arg(header.seq));
            return;
        }
        handleImage(header, payload);
    } else if (header.type == HostProtocol::kTypeMetricsJson) {
        emit metricsReceived(payload);
    } else if (header.type == HostProtocol::kTypeDetectionJson) {
        emit detectionJsonReceived(payload);
    } else if (header.type == HostProtocol::kTypeControlJson) {
        emit controlJsonReceived(payload);
    } else if (header.type == HostProtocol::kTypeSimLine) {
        const QString line = QString::fromUtf8(payload).trimmed();
        if (!line.isEmpty()) {
            emit logLineReady(QStringLiteral("板端：%1").arg(line));
        }
    } else {
        emit logLineReady(QStringLiteral("未知包类型 %1 seq=%2").arg(header.type).arg(header.seq));
    }
}

void HostNetworkWorker::handleImage(const HostProtocol::PacketHeader &header, const QByteArray &payload)
{
    if (header.version == HostProtocol::kVersionV1) {
        if (header.width == 0 || header.height == 0 || header.pixelFormat != HostProtocol::kPixelJpeg ||
            payload.size() < 2 || !payload.startsWith("\xff\xd8")) {
            emit logLineReady(QStringLiteral("丢弃 V1 JPEG seq=%1：公共头或 JPEG 无效").arg(header.seq));
            return;
        }
        ++m_imageCount;
        const quint16 confidenceX1000 = quint16(qMin<quint32>(100, header.reserved2 & 0xff) * 10);
        emit imageResultReady(header.version, header.seq, header.width, header.height,
                              header.reserved, confidenceX1000, 0, QVariantList{}, payload);
        return;
    }

    HostProtocol::ImageResultV2 result;
    QString error;
    if (!HostProtocol::parseImageResultV2(header, payload, &result, &error)) {
        emit logLineReady(QStringLiteral("丢弃 V2 图像 seq=%1：%2").arg(header.seq).arg(error));
        return;
    }

    QVariantList boxes;
    boxes.reserve(result.boxes.size());
    for (const HostProtocol::ImageBoxV2 &box : result.boxes) {
        QVariantMap item;
        item.insert(QStringLiteral("stage"), box.stage);
        item.insert(QStringLiteral("category"), box.category);
        item.insert(QStringLiteral("scoreX1000"), box.scoreX1000);
        item.insert(QStringLiteral("x1"), box.x1);
        item.insert(QStringLiteral("y1"), box.y1);
        item.insert(QStringLiteral("x2"), box.x2);
        item.insert(QStringLiteral("y2"), box.y2);
        boxes.append(item);
    }
    ++m_imageCount;
    emit imageResultReady(header.version, result.frameId, result.width, result.height,
                          result.primaryClassId, result.primaryConfidenceX1000,
                          result.inferTimeMs, boxes, result.jpeg);
}

void HostNetworkWorker::sendJsonPacket(quint16 type, const QByteArray &json)
{
    if (!m_control.socket || m_control.socket->state() != QAbstractSocket::ConnectedState) {
        return;
    }

    const QByteArray packet = HostProtocol::makeJsonPacket(type, ++m_txSeq, json);
    m_control.socket->write(packet);
    m_control.socket->flush();
}

void HostNetworkWorker::sendSimLine(const QString &line)
{
    const QString trimmed = line.trimmed();
    if (trimmed.isEmpty()) {
        return;
    }
    sendJsonPacket(HostProtocol::kTypeSimLine, trimmed.toUtf8());
}

void HostNetworkWorker::sendInitialControls()
{
    const QDateTime now = QDateTime::currentDateTime();
    QJsonObject timeObj;
    timeObj.insert(QStringLiteral("unix_ms"), QString::number(now.toMSecsSinceEpoch()));
    timeObj.insert(QStringLiteral("tz_offset_min"), now.offsetFromUtc() / 60);
    sendJsonPacket(HostProtocol::kTypeTimeSyncJson, QJsonDocument(timeObj).toJson(QJsonDocument::Compact));
    emit logLineReady(QStringLiteral("已发送时间同步"));
}

void HostNetworkWorker::emitState(const QString &status)
{
    m_statusText = status;
    const bool listening = m_controlServer.isListening() && m_imageServer.isListening();
    const bool connected = m_control.socket && m_control.socket->state() == QAbstractSocket::ConnectedState;
    emit stateChanged(listening, connected, m_statusText);
}
