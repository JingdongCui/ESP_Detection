#include "hostcontroller.h"

#include "demodatasource.h"
#include "hostnetworkworker.h"

#include <QBuffer>
#include <QDateTime>
#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcessEnvironment>
#include <QSaveFile>
#include <QStandardPaths>
#include <QThread>
#include <QUrl>
#include <QUrlQuery>

namespace {
constexpr int kLivePreviewEveryNFrames = 10;
constexpr int kDeviceResultMaxDetections = 1;
}

HostController::HostController(QObject *parent)
    : QObject(parent)
    , m_demo(new DemoDataSource(this))
{
    const QString docs = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    m_saveDir = QDir(docs.isEmpty() ? QDir::homePath() : docs).filePath(QStringLiteral("ESP32Host"));
    m_imageDir = QDir(m_saveDir).filePath(QStringLiteral("images"));
    m_modelDir = QDir(QStringLiteral("/home/kazeform/2026upper")).filePath(QStringLiteral("models"));
    ensureSaveDirs();

    m_networkThread = new QThread(this);
    m_networkWorker = new HostNetworkWorker();
    m_networkWorker->moveToThread(m_networkThread);
    connect(m_networkThread, &QThread::finished, m_networkWorker, &QObject::deleteLater);
    connect(m_networkThread, &QThread::started, m_networkWorker, &HostNetworkWorker::start);
    connect(m_networkWorker, &HostNetworkWorker::stateChanged, this, &HostController::onNetworkStateChanged);
    connect(m_networkWorker, &HostNetworkWorker::bytesReceived, this, &HostController::onNetworkBytesReceived);
    connect(m_networkWorker, &HostNetworkWorker::imageFrameSeen, this, &HostController::onNetworkImageFrameSeen);
    connect(m_networkWorker, &HostNetworkWorker::imagePreviewReady, this, &HostController::onNetworkImagePreviewReady);
    connect(m_networkWorker, &HostNetworkWorker::metricsReceived, this, &HostController::handleTelemetry);
    connect(m_networkWorker, &HostNetworkWorker::detectionJsonReceived, this, &HostController::handleDetectionJson);
    connect(m_networkWorker, &HostNetworkWorker::inferenceFrameReady, this, &HostController::onNetworkInferenceFrameReady);
    connect(m_networkWorker, &HostNetworkWorker::inferenceStatusChanged, this, &HostController::onNetworkInferenceStatusChanged);
    connect(m_networkWorker, &HostNetworkWorker::logLineReady, this, &HostController::appendLog);
    m_networkThread->start();

    connect(m_demo, &DemoDataSource::metricsReady, this, [this](const QVariantMap &metrics) {
        if (!connected()) {
            applyMetrics(metrics, true);
        }
    });
    connect(m_demo, &DemoDataSource::detectionReady, this, [this](const QVariantMap &frame) {
        if (!connected()) {
            applyDetectionFrame(frame, true);
        }
    });
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
bool HostController::demoMode() const { return !connected(); }
QString HostController::statusText() const { return m_statusText; }
QString HostController::latestImageUrl() const { return m_latestImageUrl; }
QString HostController::latestFrameInfo() const { return m_latestFrameInfo; }
QString HostController::saveDir() const { return m_saveDir; }
QString HostController::telemetryText() const { return m_telemetryText; }
int HostController::cpuUsage() const { return m_cpuUsage; }
double HostController::psramUsage() const { return m_psramUsage; }
double HostController::heapUsage() const { return m_heapUsage; }
double HostController::internalUsage() const { return m_internalUsage; }
double HostController::freeHeapMb() const { return m_freeHeapMb; }
double HostController::freeInternalMb() const { return m_freeInternalMb; }
double HostController::freePsramMb() const { return m_freePsramMb; }
double HostController::totalPsramMb() const { return m_totalPsramMb; }
double HostController::largestBlockMb() const { return m_largestBlockMb; }
double HostController::fps() const { return m_fps; }
int HostController::latencyMs() const { return m_latencyMs; }
QString HostController::uptimeText() const { return m_uptimeText; }
QString HostController::lastTelemetryTime() const { return m_lastTelemetryTime; }
quint64 HostController::bytesReceived() const { return m_bytesReceived; }
int HostController::imageCount() const { return m_imageCount; }
int HostController::detectionCount() const { return m_detectionCount; }
QVariantList HostController::metricHistory() const { return m_metricHistory; }
QVariantList HostController::frameHistory() const { return m_frameHistory; }
QVariantList HostController::currentDetections() const { return m_currentDetections; }
bool HostController::inferenceEnabled() const { return m_inferenceEnabled; }
bool HostController::inferenceBusy() const { return m_pendingInferenceRequests > 0; }
QString HostController::inferenceStatus() const { return m_inferenceStatus; }
QString HostController::inferenceServiceUrl() const { return m_inferenceServiceUrl; }
QString HostController::modelDir() const { return m_modelDir; }
QString HostController::datasetDir() const { return m_datasetDir; }
QStringList HostController::logLines() const { return m_logLines; }
int HostController::brightness() const { return m_brightness; }
int HostController::motorSpeed() const { return m_motorSpeed; }
int HostController::dangerThreshold() const { return m_dangerThreshold; }
bool HostController::lightEnabled() const { return m_lightEnabled; }
bool HostController::conveyorEnabled() const { return m_conveyorEnabled; }
bool HostController::autoRunEnabled() const { return m_autoRunEnabled; }

QVariantList HostController::dashboardCards() const
{
    QVariantList cards;
    cards.append(makeCard(QStringLiteral("连接状态"), connected() ? QStringLiteral("在线") : QStringLiteral("演示"), m_statusText, connected() ? QStringLiteral("#49d39b") : QStringLiteral("#f2b84b")));
    cards.append(makeCard(QStringLiteral("接收吞吐"), QStringLiteral("%1 MB").arg(m_bytesReceived / 1048576.0, 0, 'f', 2), QStringLiteral("累计流量"), QStringLiteral("#54b8ff")));
    cards.append(makeCard(QStringLiteral("图像帧数"), QString::number(m_imageCount), QStringLiteral("已接收"), QStringLiteral("#8da2ff")));
    cards.append(makeCard(QStringLiteral("检测目标"), QString::number(m_detectionCount), QStringLiteral("Logo 记录"), QStringLiteral("#ff7a90")));
    cards.append(makeCard(QStringLiteral("实时帧率"), QStringLiteral("%1 FPS").arg(m_fps, 0, 'f', 1), QStringLiteral("检测管线"), QStringLiteral("#49d39b")));
    cards.append(makeCard(QStringLiteral("链路延迟"), QStringLiteral("%1 ms").arg(m_latencyMs), QStringLiteral("TCP/处理"), QStringLiteral("#f2b84b")));
    cards.append(makeCard(QStringLiteral("运行时长"), m_uptimeText, QStringLiteral("下位机"), QStringLiteral("#54b8ff")));
    cards.append(makeCard(QStringLiteral("最大空闲块"), QStringLiteral("%1 MB").arg(m_largestBlockMb, 0, 'f', 2), QStringLiteral("内存碎片"), QStringLiteral("#8da2ff")));
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
              : QStringLiteral("演示模式：记录时间同步请求"));
}

void HostController::setBrightness(int value) { updateControl(QStringLiteral("brightness"), qBound(0, value, 100), true); }
void HostController::setMotorSpeed(int value) { updateControl(QStringLiteral("motor_speed"), qBound(0, value, 100), true); }
void HostController::setDangerThreshold(int value) { updateControl(QStringLiteral("danger_threshold"), qBound(1, value, 99), true); }
void HostController::setLightEnabled(bool enabled) { updateControl(QStringLiteral("fill_light"), enabled, true); }
void HostController::setConveyorEnabled(bool enabled) { updateControl(QStringLiteral("conveyor"), enabled, true); }
void HostController::setAutoRunEnabled(bool enabled) { updateControl(QStringLiteral("auto_run"), enabled, true); }

void HostController::sendControl(const QString &command, const QVariant &value)
{
    updateControl(command, value, false);
}

void HostController::setInferenceEnabled(bool enabled)
{
    Q_UNUSED(enabled);
    m_inferenceEnabled = false;
    m_inferenceStatus = QStringLiteral("无上位机推理版本");
    if (m_networkWorker) {
        QMetaObject::invokeMethod(m_networkWorker, "setInferenceEnabled", Qt::QueuedConnection, Q_ARG(bool, false));
    }
    emit inferenceChanged();
}

void HostController::setInferenceServiceUrl(const QString &url)
{
    Q_UNUSED(url);
    m_inferenceServiceUrl.clear();
    m_inferenceStatus = QStringLiteral("无上位机推理版本");
    if (m_networkWorker) {
        QMetaObject::invokeMethod(m_networkWorker, "setInferenceServiceUrl", Qt::QueuedConnection, Q_ARG(QString, QString()));
    }
    emit inferenceChanged();
}

void HostController::onNetworkStateChanged(bool listening, bool connected, const QString &statusText)
{
    m_listening = listening;
    m_connected = connected;
    m_statusText = statusText;
    emit stateChanged();
    appendLog(statusText);
}

void HostController::onNetworkBytesReceived(qint64 bytes)
{
    m_bytesReceived += quint64(bytes);
    emit statsChanged();
}

void HostController::onNetworkImageFrameSeen(quint32 frameSeq, quint16 width, quint16 height, quint16, const QString &formatText)
{
    ++m_imageCount;
    if ((m_imageCount % 30) == 1) {
        m_latestFrameInfo = QStringLiteral("%1 x %2  %3  #%4")
                                .arg(width)
                                .arg(height)
                                .arg(formatText)
                                .arg(frameSeq);
        emit statsChanged();
        emit imageChanged();
    } else {
        emit statsChanged();
    }
}

void HostController::onNetworkImagePreviewReady(quint32 frameSeq, quint16 width, quint16 height, quint16 pixelFormat, const QByteArray &payload, const QString &formatText)
{
    saveLatestPreviewImage(frameSeq, width, height, pixelFormat, payload);
    m_latestFrameInfo = QStringLiteral("%1 x %2  %3  #%4")
                            .arg(width)
                            .arg(height)
                            .arg(formatText)
                            .arg(frameSeq);
    emit statsChanged();
    emit imageChanged();
}

void HostController::onNetworkInferenceFrameReady(const QVariantMap &frame, bool updateUi, const QString &logLine)
{
    applyDetectionFrame(frame, false);
    if (updateUi) {
        m_inferenceStatus = QStringLiteral("帧 #%1 推理完成").arg(frame.value(QStringLiteral("frame_seq")).toInt());
        if (!logLine.isEmpty()) {
            appendLog(logLine);
        }
        emit inferenceChanged();
    }
}

void HostController::onNetworkInferenceStatusChanged(const QString &status, bool logLine)
{
    m_inferenceStatus = status;
    if (logLine) {
        appendLog(status);
    }
    emit inferenceChanged();
}

void HostController::pingInferenceService()
{
    m_inferenceStatus = QStringLiteral("无上位机推理版本：无需启动本地 YOLO 服务");
    appendLog(m_inferenceStatus);
    emit inferenceChanged();
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

        const QString uploadFormat = QProcessEnvironment::systemEnvironment()
                                         .value(QStringLiteral("ESP32_UPLOAD_FORMAT"), QStringLiteral("jpeg"))
                                         .trimmed()
                                         .toLower();
        if (uploadFormat == QStringLiteral("raw") || uploadFormat == QStringLiteral("rgb888")) {
            sendControlNow(QStringLiteral("upload_format"), QStringLiteral("raw"));
            appendLog(QStringLiteral("请求下位机切换为 RAW RGB888 上传"));
        } else {
            sendControlNow(QStringLiteral("upload_format"), QStringLiteral("jpeg"));
            appendLog(QStringLiteral("请求下位机切换为 JPEG 上传"));
        }
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
    appendLog(QStringLiteral("下位机已断开，自动回到演示数据"));
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
    if (header.type == HostProtocol::kTypeImageRgb888) {
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
    if (header.width == 0 || header.height == 0) {
        appendLog(QStringLiteral("图像包无效 seq=%1").arg(header.seq));
        return;
    }

    QString path;
    QString formatText;
    if (header.pixelFormat == HostProtocol::kPixelRgb888) {
        const qsizetype expected = qsizetype(header.width) * qsizetype(header.height) * 3;
        if (payload.size() != expected) {
            appendLog(QStringLiteral("RGB 图像包无效 seq=%1 bytes=%2 expected=%3").arg(header.seq).arg(payload.size()).arg(expected));
            return;
        }
        path = QDir(m_imageDir).filePath(QStringLiteral("latest_rgb888.png"));
        formatText = QStringLiteral("RGB888");
    } else if (header.pixelFormat == HostProtocol::kPixelJpeg) {
        if (payload.isEmpty() || !payload.startsWith("\xff\xd8")) {
            appendLog(QStringLiteral("JPEG 图像包无效 seq=%1 bytes=%2").arg(header.seq).arg(payload.size()));
            return;
        }
        formatText = QStringLiteral("JPEG");
    } else {
        appendLog(QStringLiteral("不支持的图像格式 seq=%1 pixel=%2").arg(header.seq).arg(header.pixelFormat));
        return;
    }

    m_imageCount++;
    requestInference(header.seq, header.width, header.height, header.pixelFormat, payload, path);

    const bool updatePreview = m_latestImageUrl.isEmpty() ||
                               (header.seq % kLivePreviewEveryNFrames) == 0;
    if (updatePreview) {
        saveLatestPreviewImage(header.seq, header.width, header.height, header.pixelFormat, payload);
        m_latestFrameInfo = QStringLiteral("%1 x %2  %3  #%4")
                                .arg(header.width)
                                .arg(header.height)
                                .arg(formatText)
                                .arg(header.seq);
        emit statsChanged();
        emit imageChanged();
    }
}

void HostController::handleTelemetry(const QByteArray &payload)
{
    m_telemetryText = QString::fromUtf8(payload);
    const QJsonDocument doc = QJsonDocument::fromJson(payload);
    if (!doc.isObject()) {
        appendLog(QStringLiteral("性能 JSON 解析失败"));
        return;
    }
    applyMetrics(doc.object().toVariantMap(), false);

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
    applyDetectionFrame(doc.object().toVariantMap(), false);
}

void HostController::requestInference(quint32 frameSeq, quint16 width, quint16 height, quint16 pixelFormat, const QByteArray &imagePayload, const QString &imagePath)
{
    Q_UNUSED(frameSeq);
    Q_UNUSED(width);
    Q_UNUSED(height);
    Q_UNUSED(pixelFormat);
    Q_UNUSED(imagePayload);
    Q_UNUSED(imagePath);
}

void HostController::sendInferenceResultToDevice(const QVariantMap &frame)
{
    if (!connected()) {
        return;
    }

    QJsonObject out;
    out.insert(QStringLiteral("frame_seq"), frame.value(QStringLiteral("frame_seq")).toInt());
    out.insert(QStringLiteral("image_width"), frame.value(QStringLiteral("image_width")).toInt());
    out.insert(QStringLiteral("image_height"), frame.value(QStringLiteral("image_height")).toInt());
    out.insert(QStringLiteral("inference_ms"), frame.value(QStringLiteral("inference_ms")).toInt());
    out.insert(QStringLiteral("decode_ms"), frame.value(QStringLiteral("decode_ms")).toInt());
    out.insert(QStringLiteral("host_ms"), frame.value(QStringLiteral("host_ms")).toInt());

    QJsonArray detections;
    const QVariantList sourceDetections = frame.value(QStringLiteral("detections")).toList();
    const int count = qMin(sourceDetections.size(), kDeviceResultMaxDetections);
    for (int i = 0; i < count; ++i) {
        const QVariantMap source = sourceDetections.at(i).toMap();
        QJsonObject det;
        det.insert(QStringLiteral("label"), source.value(QStringLiteral("label")).toString());
        det.insert(QStringLiteral("confidence"), source.value(QStringLiteral("confidence")).toDouble());
        det.insert(QStringLiteral("x"), source.value(QStringLiteral("x")).toDouble());
        det.insert(QStringLiteral("y"), source.value(QStringLiteral("y")).toDouble());
        det.insert(QStringLiteral("w"), source.value(QStringLiteral("w")).toDouble());
        det.insert(QStringLiteral("h"), source.value(QStringLiteral("h")).toDouble());
        detections.append(det);
    }
    out.insert(QStringLiteral("detections"), detections);

    const QJsonDocument doc(out);
    sendJsonPacket(HostProtocol::kTypeInferenceResultJson, doc.toJson(QJsonDocument::Compact));
}

bool HostController::saveLatestPreviewImage(quint32 frameSeq, quint16 width, quint16 height, quint16 pixelFormat, const QByteArray &imagePayload)
{
    QString path;
    QByteArray bytes;
    if (pixelFormat == HostProtocol::kPixelJpeg) {
        path = QDir(m_imageDir).filePath(QStringLiteral("latest_preview.jpg"));
        bytes = imagePayload;
    } else if (pixelFormat == HostProtocol::kPixelRgb888) {
        const qsizetype expected = qsizetype(width) * qsizetype(height) * 3;
        if (imagePayload.size() != expected) {
            return false;
        }
        QImage image(reinterpret_cast<const uchar *>(imagePayload.constData()),
                     width,
                     height,
                     int(width) * 3,
                     QImage::Format_RGB888);
        path = QDir(m_imageDir).filePath(QStringLiteral("latest_preview.png"));
        QBuffer buffer(&bytes);
        if (!buffer.open(QIODevice::WriteOnly) || !image.save(&buffer, "PNG")) {
            return false;
        }
    } else {
        return false;
    }

    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    if (file.write(bytes) != bytes.size()) {
        return false;
    }
    if (!file.commit()) {
        return false;
    }

    QUrl url = QUrl::fromLocalFile(path);
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("seq"), QString::number(frameSeq));
    url.setQuery(query);
    m_latestImageUrl = url.toString();
    return true;
}

void HostController::applyMetrics(const QVariantMap &metrics, bool fromDemo)
{
    const double bytesToMb = 1.0 / (1024.0 * 1024.0);
    m_cpuUsage = qBound(0, metrics.value(QStringLiteral("cpu_usage"), m_cpuUsage).toInt(), 100);
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
    if (fromDemo && m_statusText.isEmpty()) {
        m_statusText = QStringLiteral("演示模式：等待 ESP32 连接");
        emit stateChanged();
    }
    emit dashboardChanged();
}

void HostController::applyDetectionFrame(const QVariantMap &frame, bool fromDemo)
{
    const QVariantList detections = frame.value(QStringLiteral("detections")).toList();
    m_currentDetections = detections;
    m_detectionCount += detections.size();

    QVariantMap record;
    const int seq = frame.value(QStringLiteral("frame_seq"), m_imageCount + m_frameHistory.size() + 1).toInt();
    const int width = frame.value(QStringLiteral("image_width"), 640).toInt();
    const int height = frame.value(QStringLiteral("image_height"), 384).toInt();
    const bool updateUi = fromDemo || (seq % 10) == 0;
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
    record.insert(QStringLiteral("title"), bestLabel);
    record.insert(QStringLiteral("model"), frame.value(QStringLiteral("model"), fromDemo ? QStringLiteral("YOLO-ExpressLogo Demo") : QStringLiteral("YOLO-ExpressLogo")).toString());
    record.insert(QStringLiteral("resolution"), QStringLiteral("%1 x %2").arg(width).arg(height));
    record.insert(QStringLiteral("inferenceMs"), frame.value(QStringLiteral("inference_ms"), 0).toInt());
    record.insert(QStringLiteral("count"), detections.size());
    record.insert(QStringLiteral("confidence"), bestConfidence);
    record.insert(QStringLiteral("danger"), lowConfidence > 0);
    record.insert(QStringLiteral("detections"), detections);
    m_frameHistory.prepend(record);
    while (m_frameHistory.size() > 36) {
        m_frameHistory.removeLast();
    }

    if (fromDemo && m_latestImageUrl.isEmpty()) {
        m_latestFrameInfo = QStringLiteral("演示帧  %1 x %2  #%3").arg(width).arg(height).arg(seq);
    } else {
        m_latestFrameInfo = QStringLiteral("检测帧  %1 x %2  #%3").arg(width).arg(height).arg(seq);
    }
    appendLog(QStringLiteral("检测帧 #%1：%2 个目标，%3 个低置信度").arg(seq).arg(detections.size()).arg(lowConfidence));
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

void HostController::ensureSaveDirs()
{
    QDir().mkpath(m_imageDir);
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

void HostController::updateControl(const QString &command, const QVariant &value, bool emitSignal)
{
    bool changed = false;
    if (command == QStringLiteral("brightness")) {
        const int next = value.toInt();
        changed = m_brightness != next;
        m_brightness = next;
    } else if (command == QStringLiteral("motor_speed")) {
        const int next = value.toInt();
        changed = m_motorSpeed != next;
        m_motorSpeed = next;
    } else if (command == QStringLiteral("danger_threshold")) {
        const int next = value.toInt();
        changed = m_dangerThreshold != next;
        m_dangerThreshold = next;
    } else if (command == QStringLiteral("fill_light")) {
        const bool next = value.toBool();
        changed = m_lightEnabled != next;
        m_lightEnabled = next;
    } else if (command == QStringLiteral("conveyor")) {
        const bool next = value.toBool();
        changed = m_conveyorEnabled != next;
        m_conveyorEnabled = next;
    } else if (command == QStringLiteral("auto_run")) {
        const bool next = value.toBool();
        changed = m_autoRunEnabled != next;
        m_autoRunEnabled = next;
    } else {
        changed = true;
    }

    queueControlSend(command, value);
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
    QJsonObject obj;
    obj.insert(QStringLiteral("command"), command);
    obj.insert(QStringLiteral("request_id"), QString::number(++m_txSeq));
    obj.insert(QStringLiteral("timestamp_ms"), QString::number(QDateTime::currentMSecsSinceEpoch()));
    obj.insert(QStringLiteral("value"), QJsonValue::fromVariant(value));
    sendJsonPacket(HostProtocol::kTypeControlJson, QJsonDocument(obj).toJson(QJsonDocument::Compact));
    appendLog(QStringLiteral("%1控制：%2 = %3")
                  .arg(connected() ? QStringLiteral("已发送") : QStringLiteral("演示"))
                  .arg(command, value.toString()));
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
