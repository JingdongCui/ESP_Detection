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
    Q_PROPERTY(int dangerThreshold READ dangerThreshold NOTIFY controlsChanged)
    Q_PROPERTY(bool detectionEnabled READ detectionEnabled NOTIFY controlsChanged)
    Q_PROPERTY(bool previewOverlayEnabled READ previewOverlayEnabled NOTIFY controlsChanged)
    Q_PROPERTY(int screenBrightness READ screenBrightness NOTIFY controlsChanged)
    Q_PROPERTY(int cameraBrightness READ cameraBrightness NOTIFY controlsChanged)
    Q_PROPERTY(int cameraBrightnessMin READ cameraBrightnessMin NOTIFY controlsChanged)
    Q_PROPERTY(int cameraBrightnessMax READ cameraBrightnessMax NOTIFY controlsChanged)
    Q_PROPERTY(int cameraBrightnessStep READ cameraBrightnessStep NOTIFY controlsChanged)
    Q_PROPERTY(bool cameraBrightnessSupported READ cameraBrightnessSupported NOTIFY controlsChanged)
    Q_PROPERTY(int cameraContrast READ cameraContrast NOTIFY controlsChanged)
    Q_PROPERTY(int cameraContrastMin READ cameraContrastMin NOTIFY controlsChanged)
    Q_PROPERTY(int cameraContrastMax READ cameraContrastMax NOTIFY controlsChanged)
    Q_PROPERTY(int cameraContrastStep READ cameraContrastStep NOTIFY controlsChanged)
    Q_PROPERTY(bool cameraContrastSupported READ cameraContrastSupported NOTIFY controlsChanged)
    Q_PROPERTY(bool cameraContrastAuto READ cameraContrastAuto NOTIFY controlsChanged)
    Q_PROPERTY(int cameraSaturation READ cameraSaturation NOTIFY controlsChanged)
    Q_PROPERTY(int cameraSaturationMin READ cameraSaturationMin NOTIFY controlsChanged)
    Q_PROPERTY(int cameraSaturationMax READ cameraSaturationMax NOTIFY controlsChanged)
    Q_PROPERTY(int cameraSaturationStep READ cameraSaturationStep NOTIFY controlsChanged)
    Q_PROPERTY(bool cameraSaturationSupported READ cameraSaturationSupported NOTIFY controlsChanged)
    Q_PROPERTY(bool cameraSaturationAuto READ cameraSaturationAuto NOTIFY controlsChanged)
    Q_PROPERTY(int cameraHue READ cameraHue NOTIFY controlsChanged)
    Q_PROPERTY(int cameraHueMin READ cameraHueMin NOTIFY controlsChanged)
    Q_PROPERTY(int cameraHueMax READ cameraHueMax NOTIFY controlsChanged)
    Q_PROPERTY(int cameraHueStep READ cameraHueStep NOTIFY controlsChanged)
    Q_PROPERTY(bool cameraHueSupported READ cameraHueSupported NOTIFY controlsChanged)
    Q_PROPERTY(int waybillThreshold READ waybillThreshold NOTIFY controlsChanged)
    Q_PROPERTY(int logoThreshold READ logoThreshold NOTIFY controlsChanged)
    Q_PROPERTY(int motorASpeed READ motorASpeed NOTIFY controlsChanged)
    Q_PROPERTY(int motorBSpeed READ motorBSpeed NOTIFY controlsChanged)
    Q_PROPERTY(int motorCSpeed READ motorCSpeed NOTIFY controlsChanged)
    Q_PROPERTY(bool reportImageEnabled READ reportImageEnabled NOTIFY controlsChanged)
    Q_PROPERTY(bool reportMetricsEnabled READ reportMetricsEnabled NOTIFY controlsChanged)
    Q_PROPERTY(QString exposureText READ exposureText NOTIFY controlsChanged)
    Q_PROPERTY(QString gainText READ gainText NOTIFY controlsChanged)
    Q_PROPERTY(QString whiteBalanceText READ whiteBalanceText NOTIFY controlsChanged)
    Q_PROPERTY(QString localIp READ localIp NOTIFY controlsChanged)
    Q_PROPERTY(QString hostIp READ hostIp NOTIFY controlsChanged)
    Q_PROPERTY(QString modelInfo READ modelInfo NOTIFY controlsChanged)
    Q_PROPERTY(QString controlStatusText READ controlStatusText NOTIFY controlsChanged)

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
    int dangerThreshold() const;
    bool detectionEnabled() const;
    bool previewOverlayEnabled() const;
    int screenBrightness() const;
    int cameraBrightness() const;
    int cameraBrightnessMin() const;
    int cameraBrightnessMax() const;
    int cameraBrightnessStep() const;
    bool cameraBrightnessSupported() const;
    int cameraContrast() const;
    int cameraContrastMin() const;
    int cameraContrastMax() const;
    int cameraContrastStep() const;
    bool cameraContrastSupported() const;
    bool cameraContrastAuto() const;
    int cameraSaturation() const;
    int cameraSaturationMin() const;
    int cameraSaturationMax() const;
    int cameraSaturationStep() const;
    bool cameraSaturationSupported() const;
    bool cameraSaturationAuto() const;
    int cameraHue() const;
    int cameraHueMin() const;
    int cameraHueMax() const;
    int cameraHueStep() const;
    bool cameraHueSupported() const;
    int waybillThreshold() const;
    int logoThreshold() const;
    int motorASpeed() const;
    int motorBSpeed() const;
    int motorCSpeed() const;
    bool reportImageEnabled() const;
    bool reportMetricsEnabled() const;
    QString exposureText() const;
    QString gainText() const;
    QString whiteBalanceText() const;
    QString localIp() const;
    QString hostIp() const;
    QString modelInfo() const;
    QString controlStatusText() const;

    Q_INVOKABLE void startServer();
    Q_INVOKABLE void sendTimeSync();
    Q_INVOKABLE void setScreenBrightness(int value);
    Q_INVOKABLE void setCameraBrightness(int value);
    Q_INVOKABLE void setCameraContrast(int value);
    Q_INVOKABLE void setCameraContrastAuto(bool enabled);
    Q_INVOKABLE void setCameraSaturation(int value);
    Q_INVOKABLE void setCameraSaturationAuto(bool enabled);
    Q_INVOKABLE void setCameraHue(int value);
    Q_INVOKABLE void setWaybillThreshold(int value);
    Q_INVOKABLE void setLogoThreshold(int value);
    Q_INVOKABLE void setMotorASpeed(int value);
    Q_INVOKABLE void setMotorBSpeed(int value);
    Q_INVOKABLE void setMotorCSpeed(int value);
    Q_INVOKABLE void setReportImageEnabled(bool enabled);
    Q_INVOKABLE void setReportMetricsEnabled(bool enabled);
    Q_INVOKABLE void setDetectionEnabled(bool enabled);
    Q_INVOKABLE void setPreviewOverlayEnabled(bool enabled);
    Q_INVOKABLE void restartDevice();
    Q_INVOKABLE void requestDeviceState();
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
    void onNetworkImageResultReady(quint16 protocolVersion, quint32 frameId, quint16 width, quint16 height,
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
    void handleControlJson(const QByteArray &payload);
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
    void sendControlAction(const QString &key);
    void applyControlState(const QJsonObject &message);
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
    QString m_latestFrameInfo = QStringLiteral("无");
    QString m_latestCategoryLabel = QStringLiteral("无");
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

    int m_dangerThreshold = 50;
    bool m_detectionEnabled = true;
    bool m_previewOverlayEnabled = true;
    int m_screenBrightness = 80;
    int m_cameraBrightness = 0;
    int m_cameraBrightnessMin = 0;
    int m_cameraBrightnessMax = 255;
    int m_cameraBrightnessStep = 1;
    bool m_cameraBrightnessSupported = false;
    int m_cameraContrast = 0;
    int m_cameraContrastMin = 0;
    int m_cameraContrastMax = 255;
    int m_cameraContrastStep = 1;
    bool m_cameraContrastSupported = false;
    bool m_cameraContrastAuto = false;
    int m_cameraSaturation = 0;
    int m_cameraSaturationMin = 0;
    int m_cameraSaturationMax = 255;
    int m_cameraSaturationStep = 1;
    bool m_cameraSaturationSupported = false;
    bool m_cameraSaturationAuto = false;
    int m_cameraHue = 0;
    int m_cameraHueMin = -180;
    int m_cameraHueMax = 180;
    int m_cameraHueStep = 1;
    bool m_cameraHueSupported = false;
    int m_waybillThreshold = 50;
    int m_logoThreshold = 50;
    int m_motorASpeed = 60;
    int m_motorBSpeed = 100;
    int m_motorCSpeed = 100;
    bool m_reportImageEnabled = true;
    bool m_reportMetricsEnabled = true;
    QString m_exposureText = QStringLiteral("--");
    QString m_gainText = QStringLiteral("--");
    QString m_whiteBalanceText = QStringLiteral("--");
    QString m_localIp = QStringLiteral("192.168.10.2");
    QString m_hostIp = QStringLiteral("192.168.10.1");
    QString m_modelInfo = QStringLiteral("--");
    QString m_controlStatusText = QStringLiteral("等待设备状态");
};
