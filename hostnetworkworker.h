#pragma once

#include "packetprotocol.h"

#include <QByteArray>
#include <QObject>
#include <QPointer>
#include <QTcpServer>
#include <QTcpSocket>

class HostNetworkWorker : public QObject
{
    Q_OBJECT

public:
    explicit HostNetworkWorker(QObject *parent = nullptr);

public slots:
    void start();
    void stop();
    void sendJsonPacket(quint16 type, const QByteArray &json);
    void sendSimLine(const QString &line);

signals:
    void stateChanged(bool listening, bool connected, const QString &statusText);
    void bytesReceived(qint64 bytes);
    void imageFrameSeen(quint32 frameSeq, quint16 width, quint16 height, quint16 pixelFormat,
                        quint16 classId, quint8 confidencePct, const QString &formatText);
    void imagePreviewReady(quint32 frameSeq, quint16 width, quint16 height, quint16 pixelFormat,
                           quint16 classId, quint8 confidencePct, const QByteArray &payload, const QString &formatText);
    void metricsReceived(const QByteArray &payload);
    void detectionJsonReceived(const QByteArray &payload);
    void logLineReady(const QString &line);

private:
    struct StreamState {
        QPointer<QTcpSocket> socket;
        QByteArray buffer;
    };

    void listen();
    void acceptControlConnection();
    void acceptImageConnection();
    void configureSocket(QTcpSocket *socket, bool imageSocket);
    void onSocketDisconnected(QTcpSocket *socket, bool imageSocket);
    void readFrom(StreamState *state, bool imageStream);
    void processBuffer(StreamState *state, bool imageStream);
    void handlePacket(const HostProtocol::PacketHeader &header, const QByteArray &payload, bool imageStream);
    void handleImage(const HostProtocol::PacketHeader &header, const QByteArray &payload);
    void sendInitialControls();
    void emitState(const QString &status);

    QTcpServer m_controlServer;
    QTcpServer m_imageServer;
    StreamState m_control;
    StreamState m_image;
    QString m_statusText = QStringLiteral("未监听");
    quint32 m_txSeq = 0;
    int m_imageCount = 0;
};
