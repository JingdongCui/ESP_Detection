#include "hostcontroller.h"

#include "hostnetworkworker.h"

#include <QBuffer>
#include <QDateTime>
#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QSaveFile>
#include <QSettings>
#include <QStandardPaths>
#include <QThread>
#include <QUrl>
#include <QUrlQuery>


namespace {

bool writeBytesAtomic(const QString &path, const QByteArray &bytes)
{
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    if (file.write(bytes) != bytes.size()) {
        return false;
    }
    return file.commit();
}

QByteArray encodeJpeg(const QImage &image, int quality)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    if (!buffer.open(QIODevice::WriteOnly) || !image.save(&buffer, "JPG", quality)) {
        return {};
    }
    return bytes;
}

}

HostController::HostController(QObject *parent)
    : QObject(parent)
{
    const QString docs = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    const QString defaultSaveDir = QDir(docs.isEmpty() ? QDir::homePath() : docs)
                                       .filePath(QStringLiteral("ESP32Host"));
    m_saveDir = QSettings().value(QStringLiteral("storage/saveDir"), defaultSaveDir).toString();
    m_imageDir = QDir(m_saveDir).filePath(QStringLiteral("images"));
    if (!ensureSaveDirs()) {
        m_saveDir = defaultSaveDir;
        m_imageDir = QDir(m_saveDir).filePath(QStringLiteral("images"));
        ensureSaveDirs();
    }

    m_demoDetections = {
        QVariantMap{{QStringLiteral("label"), QStringLiteral("韵达")},
                    {QStringLiteral("confidence"), 0.87},
                    {QStringLiteral("color"), QStringLiteral("#ffe600")},
                    {QStringLiteral("x"), 0.32}, {QStringLiteral("y"), 0.24},
                    {QStringLiteral("w"), 0.34}, {QStringLiteral("h"), 0.45}}
    };
    m_demoFrameHistory = {
        QVariantMap{{QStringLiteral("seq"), 50}, {QStringLiteral("time"), QStringLiteral("演示数据")},
                    {QStringLiteral("packageLabel"), QStringLiteral("演示包裹#50")},
                    {QStringLiteral("categoryLabel"), QStringLiteral("韵达")},
                    {QStringLiteral("categoryConfidence"), 87}, {QStringLiteral("logoConfidence"), 87},
                    {QStringLiteral("title"), QStringLiteral("演示包裹#50  韵达")},
                    {QStringLiteral("model"), QStringLiteral("断连模拟视觉结果")},
                    {QStringLiteral("resolution"), QStringLiteral("1024 x 600")},
                    {QStringLiteral("processMs"), 72}, {QStringLiteral("count"), 1},
                    {QStringLiteral("confidence"), 0.87}, {QStringLiteral("danger"), false},
                    {QStringLiteral("detections"), m_demoDetections}}
    };
    const QList<QVariantMap> demoSamples = {
        {{QStringLiteral("cpu"), 35}, {QStringLiteral("psram"), 53.0}, {QStringLiteral("heap"), 44.0}, {QStringLiteral("internal"), 48.0}, {QStringLiteral("fps"), 18.0}, {QStringLiteral("latency"), 71}},
        {{QStringLiteral("cpu"), 37}, {QStringLiteral("psram"), 54.0}, {QStringLiteral("heap"), 45.0}, {QStringLiteral("internal"), 49.0}, {QStringLiteral("fps"), 18.0}, {QStringLiteral("latency"), 73}},
        {{QStringLiteral("cpu"), 39}, {QStringLiteral("psram"), 55.0}, {QStringLiteral("heap"), 46.0}, {QStringLiteral("internal"), 50.0}, {QStringLiteral("fps"), 18.0}, {QStringLiteral("latency"), 72}},
        {{QStringLiteral("cpu"), 38}, {QStringLiteral("psram"), 56.0}, {QStringLiteral("heap"), 47.0}, {QStringLiteral("internal"), 51.0}, {QStringLiteral("fps"), 18.0}, {QStringLiteral("latency"), 74}},
        {{QStringLiteral("cpu"), 36}, {QStringLiteral("psram"), 55.0}, {QStringLiteral("heap"), 46.0}, {QStringLiteral("internal"), 50.0}, {QStringLiteral("fps"), 18.0}, {QStringLiteral("latency"), 72}},
        {{QStringLiteral("cpu"), 38}, {QStringLiteral("psram"), 56.0}, {QStringLiteral("heap"), 47.0}, {QStringLiteral("internal"), 51.0}, {QStringLiteral("fps"), 18.0}, {QStringLiteral("latency"), 73}}
    };
    for (int index = 0; index < demoSamples.size(); ++index) {
        QVariantMap sample = demoSamples.at(index);
        sample.insert(QStringLiteral("time"), QStringLiteral("演示 %1").arg(index + 1));
        m_demoMetricHistory.append(sample);
    }
    m_demoLogLines = {
        QStringLiteral("演示模式：未连接板端，正在展示模拟数据"),
        QStringLiteral("演示包裹#50：韵达，识别置信度 87%"),
        QStringLiteral("综合识别正确率：96%，当前分拣速度：18 件/分钟")
    };

    m_networkThread = new QThread(this);
    m_networkWorker = new HostNetworkWorker();
    m_networkWorker->moveToThread(m_networkThread);
    connect(m_networkThread, &QThread::finished, m_networkWorker, &QObject::deleteLater);
    connect(m_networkThread, &QThread::started, m_networkWorker, &HostNetworkWorker::start);
    connect(m_networkWorker, &HostNetworkWorker::stateChanged, this, &HostController::onNetworkStateChanged);
    connect(m_networkWorker, &HostNetworkWorker::bytesReceived, this, &HostController::onNetworkBytesReceived);
    connect(m_networkWorker, &HostNetworkWorker::imageResultReady, this, &HostController::onNetworkImageResultReady);
    connect(m_networkWorker, &HostNetworkWorker::metricsReceived, this, &HostController::handleTelemetry);
    connect(m_networkWorker, &HostNetworkWorker::detectionJsonReceived, this, &HostController::handleDetectionJson);
    connect(m_networkWorker, &HostNetworkWorker::controlJsonReceived, this, &HostController::handleControlJson);
    connect(m_networkWorker, &HostNetworkWorker::logLineReady, this, &HostController::appendLog);
    m_networkThread->start();

    m_controlFlushTimer.setSingleShot(true);
    connect(&m_controlFlushTimer, &QTimer::timeout, this, &HostController::flushPendingControl);

    startServer();
}

HostController::~HostController()
{
    if (m_networkWorker) {
        QMetaObject::invokeMethod(m_networkWorker, "stop", Qt::BlockingQueuedConnection);
    }
    if (m_networkThread) {
        m_networkThread->quit();
        m_networkThread->wait();
    }
}

bool HostController::listening() const { return m_listening; }
bool HostController::connected() const { return m_connected; }
bool HostController::controlStateReady() const { return m_controlStateReady; }
QString HostController::statusText() const { return usingDemoData() ? QStringLiteral("未连接板端，展示模拟数据") : m_statusText; }
QString HostController::latestImageUrl() const { return m_latestImageUrl; }
QString HostController::latestFrameInfo() const { return usingDemoData() ? QStringLiteral("演示包裹#50  韵达") : (m_imageCount > 0 ? m_latestFrameInfo : QStringLiteral("无")); }
QString HostController::latestCategoryLabel() const { return usingDemoData() ? QStringLiteral("韵达") : (m_imageCount > 0 ? m_latestCategoryLabel : QStringLiteral("无")); }
int HostController::latestCategoryConfidence() const { return usingDemoData() ? 87 : m_latestCategoryConfidence; }
QString HostController::saveDir() const { return QDir::toNativeSeparators(m_saveDir); }
QUrl HostController::saveDirUrl() const { return QUrl::fromLocalFile(m_saveDir); }
QString HostController::telemetryText() const { return usingDemoData() ? QStringLiteral("{\"mode\":\"demo\"}") : m_telemetryText; }
int HostController::cpuUsage() const { return usingDemoData() ? 38 : m_cpuUsage; }
double HostController::psramUsage() const { return usingDemoData() ? 56.0 : m_psramUsage; }
double HostController::heapUsage() const { return usingDemoData() ? 47.0 : m_heapUsage; }
double HostController::internalUsage() const { return usingDemoData() ? 51.0 : m_internalUsage; }
double HostController::freeHeapMb() const { return usingDemoData() ? 6.36 : m_freeHeapMb; }
double HostController::freeInternalMb() const { return usingDemoData() ? 0.27 : m_freeInternalMb; }
double HostController::freePsramMb() const { return usingDemoData() ? 14.08 : m_freePsramMb; }
double HostController::totalPsramMb() const { return usingDemoData() ? 32.0 : m_totalPsramMb; }
double HostController::largestBlockMb() const { return usingDemoData() ? 4.82 : m_largestBlockMb; }
double HostController::fps() const { return usingDemoData() ? 18.0 : m_fps; }
int HostController::latencyMs() const { return usingDemoData() ? 72 : m_latencyMs; }
QString HostController::uptimeText() const { return usingDemoData() ? QStringLiteral("03:42:18") : m_uptimeText; }
QString HostController::lastTelemetryTime() const { return usingDemoData() ? QStringLiteral("演示数据") : m_lastTelemetryTime; }
quint64 HostController::bytesReceived() const { return usingDemoData() ? 7172259 : m_bytesReceived; }
int HostController::imageCount() const { return usingDemoData() ? 50 : m_imageCount; }
int HostController::detectionCount() const { return usingDemoData() ? 50 : m_detectionCount; }
QVariantList HostController::metricHistory() const { return usingDemoData() ? m_demoMetricHistory : m_metricHistory; }
QVariantList HostController::frameHistory() const { return usingDemoData() ? m_demoFrameHistory : m_frameHistory; }
QVariantList HostController::currentDetections() const { return usingDemoData() ? m_demoDetections : m_currentDetections; }
QStringList HostController::logLines() const { return usingDemoData() ? m_demoLogLines : m_logLines; }
int HostController::dangerThreshold() const { return m_dangerThreshold; }
bool HostController::detectionEnabled() const { return m_detectionEnabled; }
bool HostController::previewOverlayEnabled() const { return m_previewOverlayEnabled; }
int HostController::screenBrightness() const { return m_screenBrightness; }
int HostController::cameraBrightness() const { return m_cameraBrightness; }
int HostController::cameraBrightnessMin() const { return m_cameraBrightnessMin; }
int HostController::cameraBrightnessMax() const { return m_cameraBrightnessMax; }
int HostController::cameraBrightnessStep() const { return m_cameraBrightnessStep; }
bool HostController::cameraBrightnessSupported() const { return m_cameraBrightnessSupported; }
int HostController::cameraContrast() const { return m_cameraContrast; }
int HostController::cameraContrastMin() const { return m_cameraContrastMin; }
int HostController::cameraContrastMax() const { return m_cameraContrastMax; }
int HostController::cameraContrastStep() const { return m_cameraContrastStep; }
bool HostController::cameraContrastSupported() const { return m_cameraContrastSupported; }
bool HostController::cameraContrastAuto() const { return m_cameraContrastAuto; }
int HostController::cameraSaturation() const { return m_cameraSaturation; }
int HostController::cameraSaturationMin() const { return m_cameraSaturationMin; }
int HostController::cameraSaturationMax() const { return m_cameraSaturationMax; }
int HostController::cameraSaturationStep() const { return m_cameraSaturationStep; }
bool HostController::cameraSaturationSupported() const { return m_cameraSaturationSupported; }
bool HostController::cameraSaturationAuto() const { return m_cameraSaturationAuto; }
int HostController::cameraHue() const { return m_cameraHue; }
int HostController::cameraHueMin() const { return m_cameraHueMin; }
int HostController::cameraHueMax() const { return m_cameraHueMax; }
int HostController::cameraHueStep() const { return m_cameraHueStep; }
bool HostController::cameraHueSupported() const { return m_cameraHueSupported; }
int HostController::waybillThreshold() const { return m_waybillThreshold; }
int HostController::logoThreshold() const { return m_logoThreshold; }
bool HostController::motorOutputEnabled() const { return m_motorOutputEnabled; }
int HostController::motorASpeed() const { return m_motorASpeed; }
int HostController::motorBSpeed() const { return m_motorBSpeed; }
int HostController::motorCSpeed() const { return m_motorCSpeed; }
bool HostController::reportImageEnabled() const { return m_reportImageEnabled; }
bool HostController::reportMetricsEnabled() const { return m_reportMetricsEnabled; }
QString HostController::exposureText() const { return m_exposureText; }
QString HostController::gainText() const { return m_gainText; }
QString HostController::whiteBalanceText() const { return m_whiteBalanceText; }
QString HostController::localIp() const { return m_localIp; }
QString HostController::hostIp() const { return m_hostIp; }
QString HostController::modelInfo() const { return m_modelInfo; }
QString HostController::controlStatusText() const { return m_controlStatusText; }

QVariantList HostController::dashboardCards() const
{
    if (usingDemoData()) {
        return {
            makeCard(QStringLiteral("连接状态"), QStringLiteral("演示模式"), QStringLiteral("未连接板端"), QStringLiteral("#f2b84b")),
            makeCard(QStringLiteral("接收吞吐"), QStringLiteral("6.84 MB"), QStringLiteral("模拟累计流量"), QStringLiteral("#54b8ff")),
            makeCard(QStringLiteral("总包裹数"), QStringLiteral("50"), QStringLiteral("模拟分拣任务"), QStringLiteral("#8da2ff")),
            makeCard(QStringLiteral("最新类别"), QStringLiteral("韵达"), QStringLiteral("87% 置信度"), QStringLiteral("#ff7a90")),
            makeCard(QStringLiteral("CPU0 / CPU1"), QStringLiteral("36% / 40%"), QStringLiteral("模拟板端双核负载"), QStringLiteral("#49d39b")),
            makeCard(QStringLiteral("图片耗时"), QStringLiteral("28 / 16 ms"), QStringLiteral("模拟编码/发送"), QStringLiteral("#f2b84b")),
            makeCard(QStringLiteral("运行时长"), QStringLiteral("03:42:18"), QStringLiteral("模拟下位机"), QStringLiteral("#54b8ff")),
            makeCard(QStringLiteral("最大空闲块"), QStringLiteral("4.82 MB"), QStringLiteral("模拟内存碎片"), QStringLiteral("#8da2ff"))
        };
    }
    QVariantList cards;
    cards.append(makeCard(QStringLiteral("连接状态"), connected() ? QStringLiteral("在线") : QStringLiteral("待连接"), m_statusText, connected() ? QStringLiteral("#49d39b") : QStringLiteral("#f2b84b")));
    cards.append(makeCard(QStringLiteral("接收吞吐"), QStringLiteral("%1 MB").arg(m_bytesReceived / 1048576.0, 0, 'f', 2), QStringLiteral("累计流量"), QStringLiteral("#54b8ff")));
    cards.append(makeCard(QStringLiteral("总包裹数"), QString::number(m_imageCount), QStringLiteral("按接收图像计数"), QStringLiteral("#8da2ff")));
    const bool hasImage = m_imageCount > 0;
    cards.append(makeCard(QStringLiteral("最新类别"),
                          hasImage ? m_latestCategoryLabel : QStringLiteral("无"),
                          hasImage ? QStringLiteral("%1% 置信度").arg(m_latestCategoryConfidence)
                                   : QStringLiteral("暂无图像"),
                          QStringLiteral("#ff7a90")));
    cards.append(makeCard(QStringLiteral("CPU0 / CPU1"), QStringLiteral("%1% / %2%").arg(m_cpu0Usage).arg(m_cpu1Usage), QStringLiteral("板端双核负载"), QStringLiteral("#49d39b")));
    cards.append(makeCard(QStringLiteral("图片耗时"), QStringLiteral("%1 / %2 ms").arg(m_lastImageEncodeMs).arg(m_lastImageSendMs), QStringLiteral("编码/发送"), QStringLiteral("#f2b84b")));
    cards.append(makeCard(QStringLiteral("运行时长"), m_uptimeText, QStringLiteral("下位机"), QStringLiteral("#54b8ff")));
    cards.append(makeCard(QStringLiteral("最大空闲块"), QStringLiteral("%1 MB").arg(m_largestBlockMb, 0, 'f', 2), QStringLiteral("内存碎片"), QStringLiteral("#8da2ff")));
    return cards;
}

QVariantList HostController::courierStatsCards() const
{
    if (usingDemoData()) {
        return {
            makeCard(QStringLiteral("总包裹数"), QStringLiteral("50"), QStringLiteral("模拟分拣任务"), QStringLiteral("#49d39b")),
            makeCard(QStringLiteral("极兔"), QStringLiteral("18"), QStringLiteral("模拟类别统计"), QStringLiteral("#ff5d77")),
            makeCard(QStringLiteral("韵达"), QStringLiteral("14"), QStringLiteral("模拟类别统计，建议关注"), QStringLiteral("#e7b75d")),
            makeCard(QStringLiteral("中通"), QStringLiteral("18"), QStringLiteral("模拟类别统计"), QStringLiteral("#62b9ff"))
        };
    }
    QVariantList cards;
    cards.append(makeCard(QStringLiteral("总包裹数"), QString::number(m_imageCount), QStringLiteral("已接收包裹"), QStringLiteral("#49d39b")));
    cards.append(makeCard(QStringLiteral("极兔"), QString::number(m_jtImageCount), QStringLiteral("板端类别统计"), QStringLiteral("#ff5d77")));
    cards.append(makeCard(QStringLiteral("韵达"), QString::number(m_ydImageCount), QStringLiteral("板端类别统计"), QStringLiteral("#e7b75d")));
    cards.append(makeCard(QStringLiteral("中通"), QString::number(m_ztImageCount), QStringLiteral("板端类别统计"), QStringLiteral("#62b9ff")));
    return cards;
}

QVariantList HostController::imageLinkCards() const
{
    return courierStatsCards();
}

QVariantList HostController::imageHealthCards() const
{
    if (usingDemoData()) {
        return {
            makeCard(QStringLiteral("JPEG 发送"), QStringLiteral("50 / 50"), QStringLiteral("模拟已发送 / 已编码"), QStringLiteral("#49d39b")),
            makeCard(QStringLiteral("链路队列"), QStringLiteral("0"), QStringLiteral("模拟当前待发送图片"), QStringLiteral("#62b9ff")),
            makeCard(QStringLiteral("链路跳过"), QStringLiteral("0 / 0"), QStringLiteral("模拟背压 / 过期"), QStringLiteral("#e7b75d")),
            makeCard(QStringLiteral("快照状态"), QStringLiteral("0"), QStringLiteral("模拟无可用快照次数"), QStringLiteral("#9b8cff")),
            makeCard(QStringLiteral("最新体积"), QStringLiteral("124.6 KB"), QStringLiteral("模拟最近一次 JPEG"), QStringLiteral("#20e0c2"))
        };
    }
    QVariantList cards;
    cards.append(makeCard(QStringLiteral("JPEG 发送"), QStringLiteral("%1 / %2").arg(m_imageSent).arg(m_imageEncoded), QStringLiteral("已发送 / 已编码"), QStringLiteral("#49d39b")));
    cards.append(makeCard(QStringLiteral("链路队列"), QString::number(m_imageQueueDepth), QStringLiteral("当前待发送图片"), QStringLiteral("#62b9ff")));
    cards.append(makeCard(QStringLiteral("链路跳过"), QStringLiteral("%1 / %2").arg(m_imageDropBackpressure).arg(m_imageDropStale), QStringLiteral("背压 / 过期"), QStringLiteral("#e7b75d")));
    cards.append(makeCard(QStringLiteral("快照状态"), QString::number(m_imageNoFrame), QStringLiteral("无可用快照次数"), QStringLiteral("#9b8cff")));
    cards.append(makeCard(QStringLiteral("最新体积"), QStringLiteral("%1 KB").arg(m_lastImageBytes / 1024.0, 0, 'f', 1), QStringLiteral("最近一次 JPEG"), QStringLiteral("#20e0c2")));
    return cards;
}

void HostController::startServer()
{
    if (m_networkWorker) {
        QMetaObject::invokeMethod(m_networkWorker, "start", Qt::QueuedConnection);
    }
}

void HostController::sendTimeSync()
{
    const QDateTime now = QDateTime::currentDateTime();
    QJsonObject obj;
    obj.insert(QStringLiteral("unix_ms"), QString::number(now.toMSecsSinceEpoch()));
    obj.insert(QStringLiteral("tz_offset_min"), now.offsetFromUtc() / 60);
    sendJsonPacket(HostProtocol::kTypeTimeSyncJson, QJsonDocument(obj).toJson(QJsonDocument::Compact));
    appendLog(connected()
              ? QStringLiteral("已发送时间同步 %1").arg(now.toString(QStringLiteral("yyyy-MM-dd HH:mm:ss")))
              : QStringLiteral("待连接：已记录时间同步请求"));
}

bool HostController::setSaveDirectory(const QUrl &directoryUrl)
{
    const QString path = QDir::cleanPath(directoryUrl.toLocalFile());
    if (path.isEmpty() || path == m_saveDir) {
        return !path.isEmpty();
    }

    const QString previousSaveDir = m_saveDir;
    const QString previousImageDir = m_imageDir;
    m_saveDir = path;
    m_imageDir = QDir(m_saveDir).filePath(QStringLiteral("images"));
    if (!ensureSaveDirs()) {
        m_saveDir = previousSaveDir;
        m_imageDir = previousImageDir;
        appendLog(QStringLiteral("保存目录不可用：%1").arg(QDir::toNativeSeparators(path)));
        return false;
    }

    QSettings().setValue(QStringLiteral("storage/saveDir"), m_saveDir);
    emit saveDirChanged();
    appendLog(QStringLiteral("保存目录已更改：%1").arg(QDir::toNativeSeparators(m_saveDir)));
    return true;
}

void HostController::setDangerThreshold(int value)
{
    const int next = qBound(0, value, 100);
    if (m_dangerThreshold == next) {
        return;
    }
    m_dangerThreshold = next;
    emit controlsChanged();
}

void HostController::setScreenBrightness(int value) { updateControl(QStringLiteral("display.screen_brightness"), qBound(0, value, 100), true); }
void HostController::setCameraBrightness(int value) { updateControl(QStringLiteral("camera.brightness"), value, true); }
void HostController::setCameraContrast(int value) { updateControl(QStringLiteral("camera.contrast"), value, true); }
void HostController::setCameraContrastAuto(bool enabled) { updateControl(QStringLiteral("camera.contrast_auto"), enabled, true); }
void HostController::setCameraSaturation(int value) { updateControl(QStringLiteral("camera.saturation"), value, true); }
void HostController::setCameraSaturationAuto(bool enabled) { updateControl(QStringLiteral("camera.saturation_auto"), enabled, true); }
void HostController::setCameraHue(int value) { updateControl(QStringLiteral("camera.hue"), value, true); }
void HostController::setWaybillThreshold(int value) { updateControl(QStringLiteral("vision.waybill_threshold"), qBound(0, value, 100), true); }
void HostController::setLogoThreshold(int value) { updateControl(QStringLiteral("vision.logo_threshold"), qBound(0, value, 100), true); }
void HostController::setMotorOutputEnabled(bool enabled) { updateControl(QStringLiteral("sorter.motor_output_enabled"), enabled, true); }
void HostController::setMotorASpeed(int value) { updateControl(QStringLiteral("sorter.motor_a_speed"), qBound(0, value, 100), true); }
void HostController::setMotorBSpeed(int value) { updateControl(QStringLiteral("sorter.motor_b_speed"), qBound(0, value, 100), true); }
void HostController::setMotorCSpeed(int value) { updateControl(QStringLiteral("sorter.motor_c_speed"), qBound(0, value, 100), true); }
void HostController::setReportImageEnabled(bool enabled) { updateControl(QStringLiteral("report.image_enabled"), enabled, true); }
void HostController::setReportMetricsEnabled(bool enabled) { updateControl(QStringLiteral("report.metrics_enabled"), enabled, true); }
void HostController::setDetectionEnabled(bool enabled)
{
    updateControl(QStringLiteral("vision.detection_enabled"), enabled, false);
    updateControl(QStringLiteral("vision.preview_overlay_enabled"), enabled, false);
    updateControl(QStringLiteral("report.image_enabled"), enabled, false);
    emit controlsChanged();
    emit dashboardChanged();
}
void HostController::setPreviewOverlayEnabled(bool enabled) { updateControl(QStringLiteral("vision.preview_overlay_enabled"), enabled, true); }

void HostController::restartDevice()
{
    sendControlAction(QStringLiteral("device.restart"));
    m_controlStatusText = QStringLiteral("已发送重启命令");
    emit controlsChanged();
}

void HostController::requestDeviceState()
{
    sendJsonPacket(HostProtocol::kTypeControlJson, HostProtocol::makeControlGetJson());
    m_controlStatusText = m_connected ? QStringLiteral("正在读取设备状态") : QStringLiteral("等待设备连接");
    emit controlsChanged();
}

void HostController::sendControl(const QString &command, const QVariant &value)
{
    updateControl(command, value, false);
}

void HostController::commitPendingControls()
{
    m_controlFlushTimer.stop();
    flushPendingControl();
}

void HostController::clearFrameHistory()
{
    if (m_frameHistory.isEmpty()) {
        return;
    }
    m_frameHistory.clear();
    m_latestImageUrl.clear();
    m_latestFrameInfo = QStringLiteral("无");
    m_latestCategoryLabel = QStringLiteral("无");
    m_latestCategoryConfidence = 0;
    m_currentDetections.clear();
    emit imageChanged();
    emit detectionChanged();
}

void HostController::onNetworkStateChanged(bool listening, bool connected, const QString &statusText)
{
    const bool justConnected = connected && !m_connected;
    m_listening = listening;
    m_connected = connected;
    m_statusText = statusText;
    emit stateChanged();
    emit imageChanged();
    emit detectionChanged();
    emit statsChanged();
    emit dashboardChanged();
    emit logChanged();
    appendLog(statusText);
    if (justConnected) {
        m_controlFlushTimer.stop();
        m_pendingControls.clear();
        m_pendingControlOrder.clear();
        m_lastSentControls.clear();
        m_controlStateReady = false;
        requestDeviceState();
    } else if (!connected) {
        m_controlFlushTimer.stop();
        m_pendingControls.clear();
        m_pendingControlOrder.clear();
        m_lastSentControls.clear();
        m_controlStateReady = false;
        m_controlStatusText = QStringLiteral("设备未连接");
        emit controlsChanged();
    }
}

void HostController::onNetworkBytesReceived(qint64 bytes)
{
    m_bytesReceived += quint64(bytes);
    emit statsChanged();
}

void HostController::onNetworkImageResultReady(quint16 protocolVersion, quint32 frameId, quint16 width, quint16 height,
                                               quint16 classId, quint16 confidenceX1000, quint16 inferTimeMs,
                                               const QVariantList &boxes, const QByteArray &jpeg)
{
    const int confidencePct = qBound(0, qRound(double(confidenceX1000) / 10.0), 100);
    const QVariantList detections = makeImageDetections(width, height, boxes);
    if (!saveLatestPreviewImage(frameId, width, height, HostProtocol::kPixelJpeg, jpeg,
                                classId, confidencePct, detections)) {
        appendLog(QStringLiteral("V%1 图像保存失败 frame=%2").arg(protocolVersion).arg(frameId));
        return;
    }

    ++m_imageCount;
    m_detectionCount += detections.size();
    m_currentDetections = detections;
    if (protocolVersion == HostProtocol::kImageVersionV2) {
        m_latencyMs = inferTimeMs;
    }
    updateLatestCategory(classId, confidencePct);
    m_latestFrameInfo = protocolVersion == HostProtocol::kImageVersionV2
        ? QStringLiteral("帧#%1  %2  %3%  %4 ms  %5 个框")
              .arg(frameId).arg(m_latestCategoryLabel).arg(m_latestCategoryConfidence).arg(inferTimeMs).arg(detections.size())
        : QStringLiteral("帧#%1  %2  %3%  V1 JPEG").arg(frameId).arg(m_latestCategoryLabel).arg(m_latestCategoryConfidence);
    addImageHistoryRecord(frameId, width, height, classId, confidencePct, inferTimeMs,
                          QStringLiteral("V%1 JPEG").arg(protocolVersion), m_latestImageUrl, detections);
    emit detectionChanged();
    emit statsChanged();
    emit imageChanged();
    emit dashboardChanged();
}

void HostController::onNewConnection()
{
    while (m_server.hasPendingConnections()) {
        QTcpSocket *socket = m_server.nextPendingConnection();
        if (m_socket) {
            m_socket->disconnect(this);
            m_socket->close();
            m_socket->deleteLater();
            m_socket = nullptr;
        }
        m_socket = socket;
        m_socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
        m_buffer.clear();
        connect(m_socket, &QTcpSocket::readyRead, this, &HostController::onReadyRead);
        connect(m_socket, &QTcpSocket::disconnected, this, &HostController::onDisconnected);
        m_statusText = QStringLiteral("已连接：%1:%2")
                           .arg(m_socket->peerAddress().toString())
                           .arg(m_socket->peerPort());
        emit stateChanged();
        appendLog(m_statusText);
        sendTimeSync();
    }
}

void HostController::onReadyRead()
{
    if (!m_socket) {
        return;
    }
    const QByteArray data = m_socket->readAll();
    m_bytesReceived += quint64(data.size());
    m_buffer.append(data);
    emit statsChanged();
    processBuffer();
}

void HostController::onDisconnected()
{
    appendLog(QStringLiteral("下位机已断开，等待重新连接"));
    if (m_socket) {
        m_socket->deleteLater();
        m_socket = nullptr;
    }
    m_statusText = m_server.isListening()
        ? QStringLiteral("正在监听 %1:%2").arg(m_server.serverAddress().toString()).arg(m_server.serverPort())
        : QStringLiteral("未监听");
    emit stateChanged();
}

void HostController::processBuffer()
{
    while (m_buffer.size() >= HostProtocol::kHeaderSize) {
        HostProtocol::PacketHeader header;
        if (!HostProtocol::parseHeader(m_buffer, &header)) {
            m_buffer.remove(0, 1);
            continue;
        }
        if (header.payloadLen > HostProtocol::kMaxPayload) {
            appendLog(QStringLiteral("丢弃异常负载长度 %1").arg(header.payloadLen));
            m_buffer.remove(0, HostProtocol::kHeaderSize);
            continue;
        }
        const qsizetype packetSize = HostProtocol::kHeaderSize + qsizetype(header.payloadLen);
        if (m_buffer.size() < packetSize) {
            return;
        }
        const QByteArray payload = m_buffer.mid(HostProtocol::kHeaderSize, header.payloadLen);
        m_buffer.remove(0, packetSize);
        handlePacket(header, payload);
    }
}

void HostController::handlePacket(const HostProtocol::PacketHeader &header, const QByteArray &payload)
{
    if (header.type == HostProtocol::kTypeImageResult) {
        handleImage(header, payload);
    } else if (header.type == HostProtocol::kTypeMetricsJson) {
        handleTelemetry(payload);
    } else if (header.type == HostProtocol::kTypeDetectionJson) {
        handleDetectionJson(payload);
    } else {
        appendLog(QStringLiteral("未知包类型 %1 seq=%2").arg(header.type).arg(header.seq));
    }
}

void HostController::handleImage(const HostProtocol::PacketHeader &header, const QByteArray &payload)
{
    if (header.version == HostProtocol::kVersionV1) {
        if (header.width == 0 || header.height == 0 || header.pixelFormat != HostProtocol::kPixelJpeg ||
            payload.size() < 2 || !payload.startsWith("\xff\xd8")) {
            appendLog(QStringLiteral("丢弃 V1 JPEG seq=%1：公共头或 JPEG 无效").arg(header.seq));
            return;
        }
        onNetworkImageResultReady(header.version, header.seq, header.width, header.height,
                                  header.reserved, quint16(qMin<quint32>(100, header.reserved2 & 0xff) * 10),
                                  0, QVariantList{}, payload);
        return;
    }

    HostProtocol::ImageResultV2 result;
    QString error;
    if (!HostProtocol::parseImageResultV2(header, payload, &result, &error)) {
        appendLog(QStringLiteral("丢弃 V2 图像 seq=%1：%2").arg(header.seq).arg(error));
        return;
    }
    QVariantList boxes;
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
    onNetworkImageResultReady(header.version, result.frameId, result.width, result.height,
                              result.primaryClassId, result.primaryConfidenceX1000,
                              result.inferTimeMs, boxes, result.jpeg);
}

void HostController::handleTelemetry(const QByteArray &payload)
{
    m_telemetryText = QString::fromUtf8(payload);
    const QJsonDocument doc = QJsonDocument::fromJson(payload);
    if (!doc.isObject()) {
        appendLog(QStringLiteral("性能 JSON 解析失败"));
        return;
    }
    applyMetrics(doc.object().toVariantMap());

    QFile file(QDir(m_saveDir).filePath(QStringLiteral("telemetry.jsonl")));
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        file.write(payload);
        file.write("\n");
    }
}

void HostController::handleDetectionJson(const QByteArray &payload)
{
    const QJsonDocument doc = QJsonDocument::fromJson(payload);
    if (!doc.isObject()) {
        appendLog(QStringLiteral("检测 JSON 解析失败"));
        return;
    }
    if (m_latestImageUrl.isEmpty()) {
        applyDetectionFrame(doc.object().toVariantMap(), false);
    }
}

void HostController::handleControlJson(const QByteArray &payload)
{
    QJsonObject message;
    QString error;
    if (!HostProtocol::parseControlJson(payload, &message, &error)) {
        appendLog(QStringLiteral("控制状态解析失败：%1").arg(error));
        return;
    }
    if (message.value(QStringLiteral("op")).toString() == QStringLiteral("error")) {
        const QString key = message.value(QStringLiteral("key")).toString();
        const QString text = message.value(QStringLiteral("message")).toString();
        m_controlStatusText = key.isEmpty()
            ? QStringLiteral("设备拒绝控制：%1").arg(text)
            : QStringLiteral("%1：%2").arg(key, text);
        appendLog(m_controlStatusText);
        emit controlsChanged();
        requestDeviceState();
        return;
    }
    applyControlState(message);
}

bool HostController::saveLatestPreviewImage(quint32 frameSeq, quint16 width, quint16 height, quint16 pixelFormat,
                                            const QByteArray &imagePayload, quint16 classId, int confidencePct,
                                            const QVariantList &detections)
{
    QImage decoded;
    if (pixelFormat == HostProtocol::kPixelJpeg) {
        if (!decoded.loadFromData(imagePayload, "JPG")) {
            return false;
        }
    } else if (pixelFormat == HostProtocol::kPixelRgb888) {
        const qsizetype expected = qsizetype(width) * qsizetype(height) * 3;
        if (imagePayload.size() != expected) {
            return false;
        }
        decoded = QImage(reinterpret_cast<const uchar *>(imagePayload.constData()),
                         width,
                         height,
                         int(width) * 3,
                         QImage::Format_RGB888).copy();
    } else {
        return false;
    }

    decoded = decoded.rgbSwapped().convertToFormat(QImage::Format_RGB32);
    if (decoded.isNull()) {
        return false;
    }

    QPainter painter(&decoded);
    painter.setRenderHint(QPainter::Antialiasing);
    const qreal scale = qMax<qreal>(1.0, qMin(decoded.width(), decoded.height()) / 375.0);
    const int margin = qRound(8 * scale);
    const int lineWidth = qRound(2 * scale);
    QFont font = painter.font();
    font.setPixelSize(qRound(14 * scale));
    font.setBold(true);
    painter.setFont(font);

    const QString timestamp = QDateTime::currentDateTime().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    const QFontMetrics metrics(font);
    const QRect timeTextRect = metrics.boundingRect(timestamp);
    const QRect timeBackground(margin,
                               margin,
                               timeTextRect.width() + margin * 2,
                               timeTextRect.height() + margin);
    painter.fillRect(timeBackground, QColor(0, 0, 0, 170));
    painter.setPen(Qt::white);
    painter.drawText(timeBackground, Qt::AlignCenter, timestamp);

    int legendY = margin;
    for (const QVariant &value : detections) {
        const QVariantMap detection = value.toMap();
        const QColor color(detection.value(QStringLiteral("color")).toString());
        const int x = qRound(detection.value(QStringLiteral("x")).toDouble() * decoded.width());
        const int y = qRound(detection.value(QStringLiteral("y")).toDouble() * decoded.height());
        const int boxWidth = qRound(detection.value(QStringLiteral("w")).toDouble() * decoded.width());
        const int boxHeight = qRound(detection.value(QStringLiteral("h")).toDouble() * decoded.height());
        const QRect boxRect(x, y, qMax(1, boxWidth - 1), qMax(1, boxHeight - 1));

        painter.setPen(QPen(color, lineWidth));
        painter.drawRect(boxRect);

        const QString label = QStringLiteral("%1 %2%")
                                  .arg(detection.value(QStringLiteral("label")).toString())
                                  .arg(qRound(detection.value(QStringLiteral("confidence")).toDouble() * 100.0));
        const QRect labelBounds = metrics.boundingRect(label);
        const int badgeWidth = labelBounds.width() + margin * 2;
        const QRect labelBackground(decoded.width() - badgeWidth - margin,
                                    legendY,
                                    badgeWidth,
                                    labelBounds.height() + margin);
        painter.fillRect(labelBackground, QColor(color.red(), color.green(), color.blue(), 220));
        painter.setPen(Qt::black);
        painter.drawText(labelBackground, Qt::AlignCenter, label);
        legendY = labelBackground.bottom() + margin;
    }

    const QString result = QStringLiteral("%1 %2%")
                               .arg(categoryLabelFromId(classId))
                               .arg(qBound(0, confidencePct, 100));
    const QRect resultBounds = metrics.boundingRect(result);
    const QRect resultBackground(margin,
                                 timeBackground.bottom() + margin,
                                 resultBounds.width() + margin * 2,
                                 resultBounds.height() + margin);
    painter.fillRect(resultBackground, QColor(0, 0, 0, 170));
    painter.setPen(Qt::white);
    painter.drawText(resultBackground, Qt::AlignCenter, result);
    painter.end();

    const QByteArray bytes = encodeJpeg(decoded, 92);
    if (bytes.isEmpty() || !ensureSaveDirs()) {
        return false;
    }

    QString resultCode;
    if (classId == 1) {
        resultCode = QStringLiteral("yd");
    } else if (classId == 2) {
        resultCode = QStringLiteral("zt");
    } else {
        resultCode = QStringLiteral("jt");
    }
    const QString fileName = QStringLiteral("%1_%2.jpg")
                                 .arg(QDateTime::currentDateTime().toString(QStringLiteral("M.d_HH-mm-ss-zzz")),
                                      resultCode);
    const QString framePath = QDir(m_imageDir).filePath(fileName);
    if (!writeBytesAtomic(framePath, bytes)) {
        return false;
    }

    QUrl url = QUrl::fromLocalFile(framePath);
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("seq"), QString::number(frameSeq));
    url.setQuery(query);
    m_latestImageUrl = url.toString();
    return true;
}

QVariantList HostController::makeImageDetections(quint16 width, quint16 height, const QVariantList &boxes) const
{
    QVariantList detections;
    detections.reserve(boxes.size());
    for (int drawStage = 0; drawStage <= 1; ++drawStage) {
        for (const QVariant &value : boxes) {
            const QVariantMap item = value.toMap();
            const int stage = item.value(QStringLiteral("stage")).toInt();
            if (stage != drawStage) {
                continue;
            }

            HostProtocol::ImageBoxV2 box;
            box.stage = quint8(stage);
            box.category = quint8(item.value(QStringLiteral("category")).toUInt());
            box.scoreX1000 = quint16(item.value(QStringLiteral("scoreX1000")).toUInt());
            box.x1 = quint16(item.value(QStringLiteral("x1")).toUInt());
            box.y1 = quint16(item.value(QStringLiteral("y1")).toUInt());
            box.x2 = quint16(item.value(QStringLiteral("x2")).toUInt());
            box.y2 = quint16(item.value(QStringLiteral("y2")).toUInt());
            HostProtocol::NormalizedImageBox normalized;
            if (!HostProtocol::normalizeImageBox(width, height, box, &normalized)) {
                continue;
            }

            QVariantMap detection;
            detection.insert(QStringLiteral("stage"), stage);
            detection.insert(QStringLiteral("category"), box.category);
            detection.insert(QStringLiteral("confidence"), box.scoreX1000 / 1000.0);
            detection.insert(QStringLiteral("x"), normalized.x);
            detection.insert(QStringLiteral("y"), normalized.y);
            detection.insert(QStringLiteral("w"), normalized.width);
            detection.insert(QStringLiteral("h"), normalized.height);
            detection.insert(QStringLiteral("label"), stage == 0
                                 ? QStringLiteral("面单")
                                 : QStringLiteral("%1 Logo").arg(categoryLabelFromId(box.category)));
            detection.insert(QStringLiteral("color"), stage == 0 ? QStringLiteral("#32e843")
                                                                   : box.category == 1 ? QStringLiteral("#ffe600")
                                                                   : box.category == 2 ? QStringLiteral("#38a3ff")
                                                                                       : QStringLiteral("#ff4d5a"));
            detections.append(detection);
        }
    }
    return detections;
}

void HostController::addImageHistoryRecord(quint32 frameSeq, quint16 width, quint16 height, quint16 classId,
                                           int confidencePct, quint16 inferTimeMs, const QString &formatText,
                                           const QString &imageUrl, const QVariantList &detections)
{
    if (imageUrl.isEmpty()) {
        return;
    }

    const QString category = categoryLabelFromId(classId);
    const int confidence = qBound(0, int(confidencePct), 100);
    if (category == QStringLiteral("中通")) {
        ++m_ztImageCount;
    } else if (category == QStringLiteral("韵达")) {
        ++m_ydImageCount;
    } else {
        ++m_jtImageCount;
    }

    int logoConfidence = 0;
    for (const QVariant &value : detections) {
        const QVariantMap detection = value.toMap();
        if (detection.value(QStringLiteral("stage")).toInt() == 1) {
            logoConfidence = qMax(logoConfidence,
                                  qRound(detection.value(QStringLiteral("confidence")).toDouble() * 100.0));
        }
    }

    QVariantMap record;
    record.insert(QStringLiteral("seq"), int(frameSeq));
    record.insert(QStringLiteral("time"), QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss")));
    record.insert(QStringLiteral("imageUrl"), imageUrl);
    const QString packageLabel = QStringLiteral("包裹#%1").arg(++m_packageDisplaySeq);
    record.insert(QStringLiteral("packageLabel"), packageLabel);
    record.insert(QStringLiteral("categoryLabel"), category);
    record.insert(QStringLiteral("categoryConfidence"), confidence);
    record.insert(QStringLiteral("logoConfidence"), logoConfidence);
    record.insert(QStringLiteral("title"), QStringLiteral("%1  %2").arg(packageLabel, category));
    record.insert(QStringLiteral("model"), QStringLiteral("板端 %1 预览").arg(formatText));
    record.insert(QStringLiteral("resolution"), QStringLiteral("%1 x %2").arg(width).arg(height));
    record.insert(QStringLiteral("processMs"), inferTimeMs);
    record.insert(QStringLiteral("count"), detections.size());
    record.insert(QStringLiteral("confidence"), confidence / 100.0);
    record.insert(QStringLiteral("danger"), logoConfidence < m_dangerThreshold);
    record.insert(QStringLiteral("detections"), detections);

    m_frameHistory.prepend(record);
    while (m_frameHistory.size() > 48) {
        m_frameHistory.removeLast();
    }
}

void HostController::applyMetrics(const QVariantMap &metrics)
{
    const double bytesToMb = 1.0 / (1024.0 * 1024.0);
    m_cpuUsage = qBound(0, metrics.value(QStringLiteral("cpu_usage"), m_cpuUsage).toInt(), 100);
    m_cpu0Usage = qBound(0, metrics.value(QStringLiteral("cpu0"), m_cpu0Usage).toInt(), 100);
    m_cpu1Usage = qBound(0, metrics.value(QStringLiteral("cpu1"), m_cpu1Usage).toInt(), 100);
    m_freeHeapMb = metrics.contains(QStringLiteral("free_heap_mb"))
        ? metrics.value(QStringLiteral("free_heap_mb")).toDouble()
        : metrics.value(QStringLiteral("free_heap"), m_freeHeapMb / bytesToMb).toDouble() * bytesToMb;
    m_freeInternalMb = metrics.contains(QStringLiteral("free_internal_mb"))
        ? metrics.value(QStringLiteral("free_internal_mb")).toDouble()
        : metrics.value(QStringLiteral("free_internal"), m_freeInternalMb / bytesToMb).toDouble() * bytesToMb;
    m_freePsramMb = metrics.contains(QStringLiteral("free_psram_mb"))
        ? metrics.value(QStringLiteral("free_psram_mb")).toDouble()
        : metrics.value(QStringLiteral("free_psram"), m_freePsramMb / bytesToMb).toDouble() * bytesToMb;
    m_totalPsramMb = metrics.contains(QStringLiteral("total_psram_mb"))
        ? metrics.value(QStringLiteral("total_psram_mb")).toDouble()
        : metrics.value(QStringLiteral("total_psram"), m_totalPsramMb / bytesToMb).toDouble() * bytesToMb;
    m_largestBlockMb = metrics.contains(QStringLiteral("largest_block_mb"))
        ? metrics.value(QStringLiteral("largest_block_mb")).toDouble()
        : metrics.value(QStringLiteral("largest_free_block"), m_largestBlockMb / bytesToMb).toDouble() * bytesToMb;
    m_fps = metrics.value(QStringLiteral("fps"), m_fps).toDouble();
    m_latencyMs = metrics.value(QStringLiteral("latency_ms"), m_latencyMs).toInt();
    m_imageQueueDepth = metrics.value(QStringLiteral("image_queue_depth"), m_imageQueueDepth).toInt();
    m_imageEncoded = metrics.value(QStringLiteral("image_encoded"), m_imageEncoded).toInt();
    m_imageSent = metrics.value(QStringLiteral("image_sent"), m_imageSent).toInt();
    m_imageDropBackpressure = metrics.value(QStringLiteral("image_drop_backpressure"), m_imageDropBackpressure).toInt();
    m_imageDropStale = metrics.value(QStringLiteral("image_drop_stale"), m_imageDropStale).toInt();
    m_imageEncodeFail = metrics.value(QStringLiteral("image_encode_fail"), m_imageEncodeFail).toInt();
    m_imageSendFail = metrics.value(QStringLiteral("image_send_fail"), m_imageSendFail).toInt();
    m_imageNoFrame = metrics.value(QStringLiteral("image_no_frame"), m_imageNoFrame).toInt();
    m_lastImageEncodeMs = metrics.value(QStringLiteral("last_image_encode_ms"), m_lastImageEncodeMs).toInt();
    m_lastImageSendMs = metrics.value(QStringLiteral("last_image_send_ms"), m_lastImageSendMs).toInt();
    m_lastImageBytes = metrics.value(QStringLiteral("last_image_bytes"), m_lastImageBytes).toInt();

    if (metrics.contains(QStringLiteral("isp_exposure_us"))) {
        const int exposureUs = metrics.value(QStringLiteral("isp_exposure_us")).toInt();
        m_exposureText = exposureUs >= 0
            ? QStringLiteral("%1 ms").arg(exposureUs / 1000.0, 0, 'f', 1)
            : QStringLiteral("--");
    }
    if (metrics.contains(QStringLiteral("isp_gain_x1000"))) {
        const int gain = metrics.value(QStringLiteral("isp_gain_x1000")).toInt();
        m_gainText = gain >= 0 ? QStringLiteral("%1x").arg(gain / 1000.0, 0, 'f', 2)
                               : QStringLiteral("--");
    }
    if (metrics.contains(QStringLiteral("isp_red_gain_x1000")) &&
        metrics.contains(QStringLiteral("isp_blue_gain_x1000"))) {
        const int red = metrics.value(QStringLiteral("isp_red_gain_x1000")).toInt();
        const int blue = metrics.value(QStringLiteral("isp_blue_gain_x1000")).toInt();
        m_whiteBalanceText = red >= 0 && blue >= 0
            ? QStringLiteral("R %1 / B %2").arg(red / 1000.0, 0, 'f', 2).arg(blue / 1000.0, 0, 'f', 2)
            : QStringLiteral("--");
    }

    if (m_totalPsramMb > 0.0) {
        m_psramUsage = qBound(0.0, ((m_totalPsramMb - m_freePsramMb) * 100.0) / m_totalPsramMb, 100.0);
    }
    m_heapUsage = qBound(0.0, 100.0 - (m_freeHeapMb / 12.0 * 100.0), 100.0);
    m_internalUsage = qBound(0.0, 100.0 - (m_freeInternalMb / 0.55 * 100.0), 100.0);

    const qint64 uptimeMs = metrics.value(QStringLiteral("uptime_ms"), -1).toLongLong();
    if (uptimeMs >= 0) {
        const qint64 totalSeconds = uptimeMs / 1000;
        const qint64 hours = totalSeconds / 3600;
        const qint64 minutes = (totalSeconds % 3600) / 60;
        const qint64 seconds = totalSeconds % 60;
        m_uptimeText = QStringLiteral("%1:%2:%3")
                           .arg(hours, 2, 10, QLatin1Char('0'))
                           .arg(minutes, 2, 10, QLatin1Char('0'))
                           .arg(seconds, 2, 10, QLatin1Char('0'));
    }
    m_lastTelemetryTime = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss"));
    appendMetricHistory();
    emit dashboardChanged();
    emit controlsChanged();
}

void HostController::applyDetectionFrame(const QVariantMap &frame, bool forceUpdate)
{
    const QVariantList detections = frame.value(QStringLiteral("detections")).toList();
    m_currentDetections = detections;
    m_detectionCount += detections.size();

    QVariantMap record;
    const int seq = frame.value(QStringLiteral("frame_seq"), m_imageCount + m_frameHistory.size() + 1).toInt();
    const int width = frame.value(QStringLiteral("image_width"), 640).toInt();
    const int height = frame.value(QStringLiteral("image_height"), 384).toInt();
    const bool updateUi = forceUpdate || (seq % 10) == 0;
    int lowConfidence = 0;
    double bestConfidence = 0.0;
    QString bestLabel = QStringLiteral("快递 Logo");
    for (const QVariant &value : detections) {
        const QVariantMap item = value.toMap();
        const double confidence = item.value(QStringLiteral("confidence")).toDouble();
        if (confidence > bestConfidence) {
            bestConfidence = confidence;
            bestLabel = item.value(QStringLiteral("label"), bestLabel).toString();
        }
        if (confidence * 100.0 < m_dangerThreshold) {
            ++lowConfidence;
        }
    }

    if (!updateUi) {
        return;
    }

    record.insert(QStringLiteral("seq"), seq);
    record.insert(QStringLiteral("time"), QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss")));
    record.insert(QStringLiteral("imageUrl"), m_latestImageUrl);
    const QString packageLabel = QStringLiteral("包裹#%1").arg(++m_packageDisplaySeq);
    record.insert(QStringLiteral("packageLabel"), packageLabel);
    record.insert(QStringLiteral("title"), bestLabel.isEmpty() ? packageLabel : bestLabel);
    record.insert(QStringLiteral("model"), frame.value(QStringLiteral("model"), QStringLiteral("板端视觉链路")).toString());
    record.insert(QStringLiteral("resolution"), QStringLiteral("%1 x %2").arg(width).arg(height));
    record.insert(QStringLiteral("processMs"), frame.value(QStringLiteral("process_ms"), 0).toInt());
    record.insert(QStringLiteral("count"), detections.size());
    record.insert(QStringLiteral("confidence"), bestConfidence);
    record.insert(QStringLiteral("danger"), lowConfidence > 0);
    record.insert(QStringLiteral("detections"), detections);
    m_frameHistory.prepend(record);
    while (m_frameHistory.size() > 36) {
        m_frameHistory.removeLast();
    }

    m_latestFrameInfo = QStringLiteral("%1  %2").arg(packageLabel, bestLabel);
    appendLog(QStringLiteral("%1：%2 个目标，%3 个低置信度").arg(packageLabel).arg(detections.size()).arg(lowConfidence));
    emit detectionChanged();
    emit imageChanged();
    emit dashboardChanged();
}

void HostController::appendMetricHistory()
{
    QVariantMap sample;
    sample.insert(QStringLiteral("time"), QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss")));
    sample.insert(QStringLiteral("cpu"), m_cpuUsage);
    sample.insert(QStringLiteral("psram"), m_psramUsage);
    sample.insert(QStringLiteral("heap"), m_heapUsage);
    sample.insert(QStringLiteral("internal"), m_internalUsage);
    sample.insert(QStringLiteral("fps"), m_fps);
    sample.insert(QStringLiteral("latency"), m_latencyMs);
    m_metricHistory.append(sample);
    while (m_metricHistory.size() > 52) {
        m_metricHistory.removeFirst();
    }
}

void HostController::appendLog(const QString &line)
{
    const QString stamped = QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss  ")) + line;
    m_logLines.prepend(stamped);
    while (m_logLines.size() > 120) {
        m_logLines.removeLast();
    }
    emit logChanged();
}

bool HostController::ensureSaveDirs()
{
    return QDir().mkpath(m_imageDir);
}

void HostController::sendJsonPacket(quint16 type, const QByteArray &json)
{
    if (!m_networkWorker) {
        return;
    }

    QMetaObject::invokeMethod(m_networkWorker,
                              "sendJsonPacket",
                              Qt::QueuedConnection,
                              Q_ARG(quint16, type),
                              Q_ARG(QByteArray, json));
}

void HostController::sendSimLine(const QString &line)
{
    if (!m_networkWorker) {
        return;
    }

    QMetaObject::invokeMethod(m_networkWorker,
                              "sendSimLine",
                              Qt::QueuedConnection,
                              Q_ARG(QString, line));
}

void HostController::updateControl(const QString &command, const QVariant &value, bool emitSignal)
{
    if (!m_connected || !m_controlStateReady) {
        m_controlStatusText = m_connected
            ? QStringLiteral("正在读取设备状态")
            : QStringLiteral("设备未连接");
        emit controlsChanged();
        return;
    }

    bool changed = false;
    if (command == QStringLiteral("display.screen_brightness")) {
        const int next = qBound(0, value.toInt(), 100);
        changed = m_screenBrightness != next;
        m_screenBrightness = next;
    } else if (command == QStringLiteral("camera.brightness")) {
        const int next = qBound(m_cameraBrightnessMin, value.toInt(), m_cameraBrightnessMax);
        changed = m_cameraBrightness != next;
        m_cameraBrightness = next;
    } else if (command == QStringLiteral("camera.contrast")) {
        const int next = qBound(m_cameraContrastMin, value.toInt(), m_cameraContrastMax);
        changed = m_cameraContrast != next;
        m_cameraContrast = next;
    } else if (command == QStringLiteral("camera.contrast_auto")) {
        const bool next = value.toBool();
        changed = m_cameraContrastAuto != next;
        m_cameraContrastAuto = next;
    } else if (command == QStringLiteral("camera.saturation")) {
        const int next = qBound(m_cameraSaturationMin, value.toInt(), m_cameraSaturationMax);
        changed = m_cameraSaturation != next;
        m_cameraSaturation = next;
    } else if (command == QStringLiteral("camera.saturation_auto")) {
        const bool next = value.toBool();
        changed = m_cameraSaturationAuto != next;
        m_cameraSaturationAuto = next;
    } else if (command == QStringLiteral("camera.hue")) {
        const int next = qBound(m_cameraHueMin, value.toInt(), m_cameraHueMax);
        changed = m_cameraHue != next;
        m_cameraHue = next;
    } else if (command == QStringLiteral("vision.waybill_threshold")) {
        const int next = qBound(0, value.toInt(), 100);
        changed = m_waybillThreshold != next;
        m_waybillThreshold = next;
    } else if (command == QStringLiteral("vision.logo_threshold")) {
        const int next = qBound(0, value.toInt(), 100);
        changed = m_logoThreshold != next;
        m_logoThreshold = next;
    } else if (command == QStringLiteral("sorter.motor_output_enabled")) {
        const bool next = value.toBool();
        changed = m_motorOutputEnabled != next;
        m_motorOutputEnabled = next;
    } else if (command == QStringLiteral("sorter.motor_a_speed")) {
        const int next = qBound(0, value.toInt(), 100);
        changed = m_motorASpeed != next;
        m_motorASpeed = next;
    } else if (command == QStringLiteral("sorter.motor_b_speed")) {
        const int next = qBound(0, value.toInt(), 100);
        changed = m_motorBSpeed != next;
        m_motorBSpeed = next;
    } else if (command == QStringLiteral("sorter.motor_c_speed")) {
        const int next = qBound(0, value.toInt(), 100);
        changed = m_motorCSpeed != next;
        m_motorCSpeed = next;
    } else if (command == QStringLiteral("report.image_enabled")) {
        const bool next = value.toBool();
        changed = m_reportImageEnabled != next;
        m_reportImageEnabled = next;
    } else if (command == QStringLiteral("report.metrics_enabled")) {
        const bool next = value.toBool();
        changed = m_reportMetricsEnabled != next;
        m_reportMetricsEnabled = next;
    } else if (command == QStringLiteral("vision.detection_enabled")) {
        const bool next = value.toBool();
        changed = m_detectionEnabled != next;
        m_detectionEnabled = next;
    } else if (command == QStringLiteral("vision.preview_overlay_enabled")) {
        const bool next = value.toBool();
        changed = m_previewOverlayEnabled != next;
        m_previewOverlayEnabled = next;
    } else {
        changed = true;
    }

    queueControlSend(command, value);
    m_controlStatusText = m_connected ? QStringLiteral("控制命令已发送") : QStringLiteral("设备未连接");
    if (emitSignal && changed) {
        emit controlsChanged();
        emit dashboardChanged();
    }
}

void HostController::queueControlSend(const QString &command, const QVariant &value)
{
    if (m_pendingControls.value(command) == value) {
        return;
    }
    if (!m_pendingControls.contains(command) && m_lastSentControls.value(command) == value) {
        return;
    }

    if (!m_pendingControls.contains(command)) {
        m_pendingControlOrder.append(command);
    }
    m_pendingControls.insert(command, value);

    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    const qint64 remainingMs = 100 - (now - m_lastControlSentMs);
    if (remainingMs <= 0 && !m_controlFlushTimer.isActive()) {
        flushPendingControl();
    } else if (!m_controlFlushTimer.isActive()) {
        m_controlFlushTimer.start(int(remainingMs));
    }
}

void HostController::flushPendingControl()
{
    while (!m_pendingControlOrder.isEmpty()) {
        const QString command = m_pendingControlOrder.takeFirst();
        const QVariant value = m_pendingControls.take(command);
        if (m_lastSentControls.value(command) == value) {
            continue;
        }

        sendControlNow(command, value);
        m_lastSentControls.insert(command, value);
        m_lastControlSentMs = QDateTime::currentMSecsSinceEpoch();
        break;
    }

    if (!m_pendingControlOrder.isEmpty()) {
        m_controlFlushTimer.start(100);
    }
}

void HostController::sendControlNow(const QString &command, const QVariant &value)
{
    sendJsonPacket(HostProtocol::kTypeControlJson,
                   HostProtocol::makeControlSetJson(command, QJsonValue::fromVariant(value)));
    appendLog(QStringLiteral("%1：%2 = %3")
                  .arg(connected() ? QStringLiteral("已发送控制") : QStringLiteral("未连接控制"),
                       command, value.toString()));
}

void HostController::sendControlAction(const QString &key)
{
    sendJsonPacket(HostProtocol::kTypeControlJson, HostProtocol::makeControlActionJson(key));
    appendLog(QStringLiteral("%1：%2")
                  .arg(connected() ? QStringLiteral("已发送动作") : QStringLiteral("未连接动作"), key));
}

void HostController::applyControlState(const QJsonObject &message)
{
    const QJsonObject settings = message.value(QStringLiteral("settings")).toObject();
    const QJsonObject capabilities = message.value(QStringLiteral("capabilities")).toObject();

    auto setInt = [&settings](const QString &key, int *target) {
        const QJsonValue value = settings.value(key);
        if (value.isDouble()) {
            *target = value.toInt();
        }
    };
    auto setBool = [&settings](const QString &key, bool *target) {
        const QJsonValue value = settings.value(key);
        if (value.isBool()) {
            *target = value.toBool();
        }
    };
    auto setString = [&settings](const QString &key, QString *target) {
        const QJsonValue value = settings.value(key);
        if (value.isString()) {
            *target = value.toString();
        }
    };
    auto setCapability = [&capabilities](const QString &key, bool *supported,
                                         int *minimum, int *maximum, int *step) {
        const QJsonObject object = capabilities.value(key).toObject();
        if (object.isEmpty()) {
            return;
        }
        *supported = object.value(QStringLiteral("supported")).toBool(false);
        *minimum = object.value(QStringLiteral("min")).toInt(*minimum);
        *maximum = object.value(QStringLiteral("max")).toInt(*maximum);
        *step = qMax(1, object.value(QStringLiteral("step")).toInt(*step));
    };

    setInt(QStringLiteral("display.screen_brightness"), &m_screenBrightness);
    setInt(QStringLiteral("camera.brightness"), &m_cameraBrightness);
    setInt(QStringLiteral("camera.contrast"), &m_cameraContrast);
    setBool(QStringLiteral("camera.contrast_auto"), &m_cameraContrastAuto);
    setInt(QStringLiteral("camera.saturation"), &m_cameraSaturation);
    setBool(QStringLiteral("camera.saturation_auto"), &m_cameraSaturationAuto);
    setInt(QStringLiteral("camera.hue"), &m_cameraHue);
    setInt(QStringLiteral("vision.waybill_threshold"), &m_waybillThreshold);
    setInt(QStringLiteral("vision.logo_threshold"), &m_logoThreshold);
    setBool(QStringLiteral("vision.detection_enabled"), &m_detectionEnabled);
    setBool(QStringLiteral("vision.preview_overlay_enabled"), &m_previewOverlayEnabled);
    setBool(QStringLiteral("sorter.motor_output_enabled"), &m_motorOutputEnabled);
    setInt(QStringLiteral("sorter.motor_a_speed"), &m_motorASpeed);
    setInt(QStringLiteral("sorter.motor_b_speed"), &m_motorBSpeed);
    setInt(QStringLiteral("sorter.motor_c_speed"), &m_motorCSpeed);
    setBool(QStringLiteral("report.image_enabled"), &m_reportImageEnabled);
    setBool(QStringLiteral("report.metrics_enabled"), &m_reportMetricsEnabled);
    setString(QStringLiteral("network.local_ip"), &m_localIp);
    setString(QStringLiteral("network.host_ip"), &m_hostIp);
    setString(QStringLiteral("model.info"), &m_modelInfo);

    setCapability(QStringLiteral("camera.brightness"), &m_cameraBrightnessSupported,
                  &m_cameraBrightnessMin, &m_cameraBrightnessMax, &m_cameraBrightnessStep);
    setCapability(QStringLiteral("camera.contrast"), &m_cameraContrastSupported,
                  &m_cameraContrastMin, &m_cameraContrastMax, &m_cameraContrastStep);
    setCapability(QStringLiteral("camera.saturation"), &m_cameraSaturationSupported,
                  &m_cameraSaturationMin, &m_cameraSaturationMax, &m_cameraSaturationStep);
    setCapability(QStringLiteral("camera.hue"), &m_cameraHueSupported,
                  &m_cameraHueMin, &m_cameraHueMax, &m_cameraHueStep);

    m_controlStateReady = true;
    m_controlStatusText = QStringLiteral("设备状态已同步");
    m_lastSentControls.clear();
    emit controlsChanged();
    emit dashboardChanged();
}

QVariantMap HostController::makeCard(const QString &title, const QString &value, const QString &note, const QString &accent) const
{
    QVariantMap card;
    card.insert(QStringLiteral("title"), title);
    card.insert(QStringLiteral("value"), value);
    card.insert(QStringLiteral("note"), note);
    card.insert(QStringLiteral("accent"), accent);
    return card;
}

QString HostController::categoryLabelFromId(int classId) const
{
    if (classId == 1) {
        return QStringLiteral("韵达");
    }
    if (classId == 2) {
        return QStringLiteral("中通");
    }
    return QStringLiteral("极兔");
}

void HostController::updateLatestCategory(int classId, int confidencePct)
{
    m_latestCategoryLabel = categoryLabelFromId(classId);
    m_latestCategoryConfidence = qBound(0, confidencePct, 100);
}

bool HostController::usingDemoData() const
{
    return !m_connected;
}
