#pragma once

#include "packetprotocol.h"

#include <QByteArray>
#include <QHash>
#include <QNetworkAccessManager>
#include <QObject>
#include <QPointer>
#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QVariant>
#include <QVariantList>

class DemoDataSource;

class HostController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool listening READ listening NOTIFY stateChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY stateChanged)
    Q_PROPERTY(bool demoMode READ demoMode NOTIFY stateChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY stateChanged)
    Q_PROPERTY(QString latestImageUrl READ latestImageUrl NOTIFY imageChanged)
    Q_PROPERTY(QString latestFrameInfo READ latestFrameInfo NOTIFY imageChanged)
    Q_PROPERTY(QString saveDir READ saveDir CONSTANT)
    Q_PROPERTY(QString telemetryText READ telemetryText NOTIFY dashboardChanged)
    Q_PROPERTY(int cpuUsage READ cpuUsage NOTIFY dashboardChanged)
    Q_PROPERTY(double psramUsage READ psramUsage NOTIFY dashboardChanged)
    Q_PROPERTY(double heapUsage READ heapUsage NOTIFY dashboardChanged)
    Q_PROPERTY(double internalUsage READ internalUsage NOTIFY dashboardChanged)
    Q_PROPERTY(double freeHeapMb READ freeHeapMb NOTIFY dashboardChanged)
    Q_PROPERTY(double freeInternalMb READ freeInternalMb NOTIFY dashboardChanged)
    Q_PROPERTY(double freePsramMb READ freePsramMb NOTIFY dashboardChanged)
    Q_PROPERTY(double totalPsramMb READ totalPsramMb NOTIFY dashboardChanged)
    Q_PROPERTY(double largestBlockMb READ largestBlockMb NOTIFY dashboardChanged)
    Q_PROPERTY(double fps READ fps NOTIFY dashboardChanged)
    Q_PROPERTY(int latencyMs READ latencyMs NOTIFY dashboardChanged)
    Q_PROPERTY(QString uptimeText READ uptimeText NOTIFY dashboardChanged)
    Q_PROPERTY(QString lastTelemetryTime READ lastTelemetryTime NOTIFY dashboardChanged)
    Q_PROPERTY(quint64 bytesReceived READ bytesReceived NOTIFY statsChanged)
    Q_PROPERTY(int imageCount READ imageCount NOTIFY statsChanged)
    Q_PROPERTY(int detectionCount READ detectionCount NOTIFY detectionChanged)
    Q_PROPERTY(QVariantList metricHistory READ metricHistory NOTIFY dashboardChanged)
    Q_PROPERTY(QVariantList dashboardCards READ dashboardCards NOTIFY dashboardChanged)
    Q_PROPERTY(QVariantList frameHistory READ frameHistory NOTIFY detectionChanged)
    Q_PROPERTY(QVariantList currentDetections READ currentDetections NOTIFY detectionChanged)
    Q_PROPERTY(bool inferenceEnabled READ inferenceEnabled WRITE setInferenceEnabled NOTIFY inferenceChanged)
    Q_PROPERTY(bool inferenceBusy READ inferenceBusy NOTIFY inferenceChanged)
    Q_PROPERTY(QString inferenceStatus READ inferenceStatus NOTIFY inferenceChanged)
    Q_PROPERTY(QString inferenceServiceUrl READ inferenceServiceUrl WRITE setInferenceServiceUrl NOTIFY inferenceChanged)
    Q_PROPERTY(QString modelDir READ modelDir CONSTANT)
    Q_PROPERTY(QString datasetDir READ datasetDir CONSTANT)
    Q_PROPERTY(QStringList logLines READ logLines NOTIFY logChanged)
    Q_PROPERTY(int brightness READ brightness NOTIFY controlsChanged)
    Q_PROPERTY(int motorSpeed READ motorSpeed NOTIFY controlsChanged)
    Q_PROPERTY(int dangerThreshold READ dangerThreshold NOTIFY controlsChanged)
    Q_PROPERTY(bool lightEnabled READ lightEnabled NOTIFY controlsChanged)
    Q_PROPERTY(bool conveyorEnabled READ conveyorEnabled NOTIFY controlsChanged)
    Q_PROPERTY(bool autoRunEnabled READ autoRunEnabled NOTIFY controlsChanged)

public:
    explicit HostController(QObject *parent = nullptr);

    bool listening() const;
    bool connected() const;
    bool demoMode() const;
    QString statusText() const;
    QString latestImageUrl() const;
    QString latestFrameInfo() const;
    QString saveDir() const;
    QString telemetryText() const;
    int cpuUsage() const;
    double psramUsage() const;
    double heapUsage() const;
    double internalUsage() const;
    double freeHeapMb() const;
    double freeInternalMb() const;
    double freePsramMb() const;
    double totalPsramMb() const;
    double largestBlockMb() const;
    double fps() const;
    int latencyMs() const;
    QString uptimeText() const;
    QString lastTelemetryTime() const;
    quint64 bytesReceived() const;
    int imageCount() const;
    int detectionCount() const;
    QVariantList metricHistory() const;
    QVariantList dashboardCards() const;
    QVariantList frameHistory() const;
    QVariantList currentDetections() const;
    bool inferenceEnabled() const;
    bool inferenceBusy() const;
    QString inferenceStatus() const;
    QString inferenceServiceUrl() const;
    QString modelDir() const;
    QString datasetDir() const;
    QStringList logLines() const;
    int brightness() const;
    int motorSpeed() const;
    int dangerThreshold() const;
    bool lightEnabled() const;
    bool conveyorEnabled() const;
    bool autoRunEnabled() const;

    Q_INVOKABLE void startServer();
    Q_INVOKABLE void pingInferenceService();
    Q_INVOKABLE void sendTimeSync();
    void setInferenceEnabled(bool enabled);
    void setInferenceServiceUrl(const QString &url);
    Q_INVOKABLE void setBrightness(int value);
    Q_INVOKABLE void setMotorSpeed(int value);
    Q_INVOKABLE void setDangerThreshold(int value);
    Q_INVOKABLE void setLightEnabled(bool enabled);
    Q_INVOKABLE void setConveyorEnabled(bool enabled);
    Q_INVOKABLE void setAutoRunEnabled(bool enabled);
    Q_INVOKABLE void sendControl(const QString &command, const QVariant &value);

signals:
    void stateChanged();
    void imageChanged();
    void dashboardChanged();
    void detectionChanged();
    void statsChanged();
    void logChanged();
    void controlsChanged();
    void inferenceChanged();

private:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
    void processBuffer();
    void handlePacket(const HostProtocol::PacketHeader &header, const QByteArray &payload);
    void handleImage(const HostProtocol::PacketHeader &header, const QByteArray &payload);
    void handleTelemetry(const QByteArray &payload);
    void handleDetectionJson(const QByteArray &payload);
    void requestInference(quint32 frameSeq, quint16 width, quint16 height, quint16 pixelFormat, const QByteArray &imagePayload, const QString &imagePath);
    void sendInferenceResultToDevice(const QVariantMap &frame);
    void applyMetrics(const QVariantMap &metrics, bool fromDemo);
    void applyDetectionFrame(const QVariantMap &frame, bool fromDemo);
    void appendMetricHistory();
    void appendLog(const QString &line);
    void ensureSaveDirs();
    void sendJsonPacket(quint16 type, const QByteArray &json);
    void updateControl(const QString &command, const QVariant &value, bool emitSignal);
    void queueControlSend(const QString &command, const QVariant &value);
    void flushPendingControl();
    void sendControlNow(const QString &command, const QVariant &value);
    QVariantMap makeCard(const QString &title, const QString &value, const QString &note, const QString &accent) const;

    QTcpServer m_server;
    QPointer<QTcpSocket> m_socket;
    DemoDataSource *m_demo = nullptr;
    QByteArray m_buffer;
    QString m_statusText;
    QString m_latestImageUrl;
    QString m_latestFrameInfo = QStringLiteral("--");
    QString m_saveDir;
    QString m_imageDir;
    QString m_telemetryText = QStringLiteral("{}");
    QString m_uptimeText = QStringLiteral("--");
    QString m_lastTelemetryTime = QStringLiteral("--");
    QStringList m_logLines;
    QVariantList m_metricHistory;
    QVariantList m_frameHistory;
    QVariantList m_currentDetections;
    QNetworkAccessManager m_network;
    QTimer m_controlFlushTimer;
    QHash<QString, QVariant> m_pendingControls;
    QHash<QString, QVariant> m_lastSentControls;
    QStringList m_pendingControlOrder;
    qint64 m_lastControlSentMs = 0;
    quint64 m_bytesReceived = 0;
    int m_imageCount = 0;
    int m_detectionCount = 0;
    quint32 m_txSeq = 0;
    int m_pendingInferenceRequests = 0;
    bool m_inferenceEnabled = true;
    QString m_inferenceStatus = QStringLiteral("等待推理服务");
    QString m_inferenceServiceUrl = QStringLiteral("http://127.0.0.1:8765");
    QString m_modelDir;
    QString m_datasetDir = QStringLiteral("/home/kazeform/2026upper/datasets");

    int m_cpuUsage = 0;
    double m_psramUsage = 0.0;
    double m_heapUsage = 0.0;
    double m_internalUsage = 0.0;
    double m_freeHeapMb = 0.0;
    double m_freeInternalMb = 0.0;
    double m_freePsramMb = 0.0;
    double m_totalPsramMb = 0.0;
    double m_largestBlockMb = 0.0;
    double m_fps = 0.0;
    int m_latencyMs = 0;

    int m_brightness = 68;
    int m_motorSpeed = 100;
    int m_dangerThreshold = 50;
    bool m_lightEnabled = true;
    bool m_conveyorEnabled = false;
    bool m_autoRunEnabled = true;
};
