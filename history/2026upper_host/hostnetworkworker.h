#pragma once

#include "packetprotocol.h"

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QObject>
#include <QPointer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVariantMap>

class HostNetworkWorker : public QObject
{
    Q_OBJECT

public:
    explicit HostNetworkWorker(QObject *parent = nullptr);

public slots:
    void start();
    void stop();
    void setInferenceEnabled(bool enabled);
    void setInferenceServiceUrl(const QString &url);
    void sendJsonPacket(quint16 type, const QByteArray &json);

signals:
    void stateChanged(bool listening, bool connected, const QString &statusText);
    void bytesReceived(qint64 bytes);
    void imageFrameSeen(quint32 frameSeq, quint16 width, quint16 height, quint16 pixelFormat, const QString &formatText);
    void imagePreviewReady(quint32 frameSeq, quint16 width, quint16 height, quint16 pixelFormat, const QByteArray &payload, const QString &formatText);
    void metricsReceived(const QByteArray &payload);
    void detectionJsonReceived(const QByteArray &payload);
    void inferenceFrameReady(const QVariantMap &frame, bool updateUi, const QString &logLine);
    void inferenceStatusChanged(const QString &status, bool logLine);
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
    void requestInference(quint32 frameSeq, quint16 width, quint16 height, quint16 pixelFormat, const QByteArray &imagePayload);
    void sendInferenceResultToDevice(const QVariantMap &frame);
    void sendInitialControls();
    void emitState(const QString &status);

    QTcpServer m_controlServer;
    QTcpServer m_imageServer;
    StreamState m_control;
    StreamState m_image;
    QNetworkAccessManager m_network;
    QString m_statusText = QStringLiteral("未监听");
    QString m_inferenceServiceUrl = QStringLiteral("http://127.0.0.1:8765");
    quint32 m_txSeq = 0;
    int m_imageCount = 0;
    int m_pendingInferenceRequests = 0;
    bool m_inferenceEnabled = true;
};
