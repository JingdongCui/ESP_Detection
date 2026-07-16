#pragma once

#include "packetprotocol.h"

#include <QByteArray>
#include <QHash>
#include <QObject>
#include <QPointer>
#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QVariant>
#include <QVariantList>

class HostNetworkWorker;
class QThread;

class HostController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool listening READ listening NOTIFY stateChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY stateChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY stateChanged)
    Q_PROPERTY(QString latestImageUrl READ latestImageUrl NOTIFY imageChanged)
    Q_PROPERTY(QString latestFrameInfo READ latestFrameInfo NOTIFY imageChanged)
    Q_PROPERTY(QString latestCategoryLabel READ latestCategoryLabel NOTIFY imageChanged)
    Q_PROPERTY(int latestCategoryConfidence READ latestCategoryConfidence NOTIFY imageChanged)
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
    Q_PROPERTY(QVariantList courierStatsCards READ courierStatsCards NOTIFY dashboardChanged)
    Q_PROPERTY(QVariantList imageLinkCards READ imageLinkCards NOTIFY dashboardChanged)
    Q_PROPERTY(QVariantList imageHealthCards READ imageHealthCards NOTIFY dashboardChanged)
    Q_PROPERTY(QVariantList frameHistory READ frameHistory NOTIFY detectionChanged)
    Q_PROPERTY(QVariantList currentDetections READ currentDetections NOTIFY detectionChanged)
    Q_PROPERTY(QStringList logLines READ logLines NOTIFY logChanged)
    Q_PROPERTY(int brightness READ brightness NOTIFY controlsChanged)
    Q_PROPERTY(int motorSpeed READ motorSpeed NOTIFY controlsChanged)
    Q_PROPERTY(int dangerThreshold READ dangerThreshold NOTIFY controlsChanged)
    Q_PROPERTY(bool detectionEnabled READ detectionEnabled NOTIFY controlsChanged)
    Q_PROPERTY(bool previewOverlayEnabled READ previewOverlayEnabled NOTIFY controlsChanged)

public:
    explicit HostController(QObject *parent = nullptr);
    ~HostController() override;

    bool listening() const;
    bool connected() const;
    QString statusText() const;
    QString latestImageUrl() const;
    QString latestFrameInfo() const;
    QString latestCategoryLabel() const;
    int latestCategoryConfidence() const;
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
    QVariantList courierStatsCards() const;
    QVariantList imageLinkCards() const;
    QVariantList imageHealthCards() const;
    QVariantList frameHistory() const;
    QVariantList currentDetections() const;
    QStringList logLines() const;
    int brightness() const;
    int motorSpeed() const;
    int dangerThreshold() const;
    bool detectionEnabled() const;
    bool previewOverlayEnabled() const;

    Q_INVOKABLE void startServer();
    Q_INVOKABLE void sendTimeSync();
    Q_INVOKABLE void setBrightness(int value);
    Q_INVOKABLE void setMotorSpeed(int value);
    Q_INVOKABLE void setDangerThreshold(int value);
    Q_INVOKABLE void setDetectionEnabled(bool enabled);
    Q_INVOKABLE void setPreviewOverlayEnabled(bool enabled);
    Q_INVOKABLE void sendControl(const QString &command, const QVariant &value);
    Q_INVOKABLE void clearFrameHistory();

signals:
    void stateChanged();
    void imageChanged();
    void dashboardChanged();
    void detectionChanged();
    void statsChanged();
    void logChanged();
    void controlsChanged();

private:
    void onNetworkStateChanged(bool listening, bool connected, const QString &statusText);
    void onNetworkBytesReceived(qint64 bytes);
    void onNetworkImageResultReady(quint32 frameId, quint16 width, quint16 height,
                                   quint16 classId, quint16 confidenceX1000, quint16 inferTimeMs,
                                   const QVariantList &boxes, const QByteArray &jpeg);
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
    void processBuffer();
    void handlePacket(const HostProtocol::PacketHeader &header, const QByteArray &payload);
    void handleImage(const HostProtocol::PacketHeader &header, const QByteArray &payload);
    void handleTelemetry(const QByteArray &payload);
    void handleDetectionJson(const QByteArray &payload);
    bool saveLatestPreviewImage(quint32 frameSeq, quint16 width, quint16 height, quint16 pixelFormat, const QByteArray &imagePayload);
    QVariantList makeImageDetections(quint16 width, quint16 height, const QVariantList &boxes) const;
    void addImageHistoryRecord(quint32 frameSeq, quint16 width, quint16 height, quint16 classId,
                               int confidencePct, quint16 inferTimeMs, const QString &formatText,
                               const QString &imageUrl, const QVariantList &detections);
    void applyMetrics(const QVariantMap &metrics);
    void applyDetectionFrame(const QVariantMap &frame, bool forceUpdate);
    void appendMetricHistory();
    void appendLog(const QString &line);
    void ensureSaveDirs();
    void sendJsonPacket(quint16 type, const QByteArray &json);
    void sendSimLine(const QString &line);
    void updateControl(const QString &command, const QVariant &value, bool emitSignal);
    void queueControlSend(const QString &command, const QVariant &value);
    void flushPendingControl();
    void sendControlNow(const QString &command, const QVariant &value);
    QVariantMap makeCard(const QString &title, const QString &value, const QString &note, const QString &accent) const;
    QString categoryLabelFromId(int classId) const;
    void updateLatestCategory(int classId, int confidencePct);

    QTcpServer m_server;
    QPointer<QTcpSocket> m_socket;
    QThread *m_networkThread = nullptr;
    HostNetworkWorker *m_networkWorker = nullptr;
    QByteArray m_buffer;
    QString m_statusText;
    QString m_latestImageUrl;
    QString m_latestFrameInfo = QStringLiteral("--");
    QString m_latestCategoryLabel = QStringLiteral("极兔");
    int m_latestCategoryConfidence = 0;
    QString m_saveDir;
    QString m_imageDir;
    QString m_telemetryText = QStringLiteral("{}");
    QString m_uptimeText = QStringLiteral("--");
    QString m_lastTelemetryTime = QStringLiteral("--");
    QStringList m_logLines;
    QVariantList m_metricHistory;
    QVariantList m_frameHistory;
    QVariantList m_currentDetections;
    QTimer m_controlFlushTimer;
    QHash<QString, QVariant> m_pendingControls;
    QHash<QString, QVariant> m_lastSentControls;
    QStringList m_pendingControlOrder;
    qint64 m_lastControlSentMs = 0;
    quint64 m_bytesReceived = 0;
    int m_imageCount = 0;
    int m_detectionCount = 0;
    int m_packageDisplaySeq = 0;
    quint32 m_txSeq = 0;
    bool m_listening = false;
    bool m_connected = false;

    int m_cpuUsage = 0;
    int m_cpu0Usage = 0;
    int m_cpu1Usage = 0;
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
    int m_imageQueueDepth = 0;
    int m_imageEncoded = 0;
    int m_imageSent = 0;
    int m_imageDropBackpressure = 0;
    int m_imageDropStale = 0;
    int m_imageEncodeFail = 0;
    int m_imageSendFail = 0;
    int m_imageNoFrame = 0;
    int m_lastImageEncodeMs = 0;
    int m_lastImageSendMs = 0;
    int m_lastImageBytes = 0;
    int m_jtImageCount = 0;
    int m_ztImageCount = 0;
    int m_ydImageCount = 0;

    int m_brightness = 68;
    int m_motorSpeed = 100;
    int m_dangerThreshold = 50;
    bool m_detectionEnabled = true;
    bool m_previewOverlayEnabled = true;
};
