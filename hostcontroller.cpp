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
#include <QSaveFile>
#include <QStandardPaths>
#include <QThread>
#include <QUrl>
#include <QUrlQuery>

#include <algorithm>
#include <array>

namespace {

int median9(std::array<int, 9> values)
{
    std::sort(values.begin(), values.end());
    return values[4];
}

int clampColor(int value)
{
    return qBound(0, value, 255);
}

QImage enhancePreviewImage(const QImage &source)
{
    if (source.isNull()) {
        return {};
    }

    const QImage input = source.convertToFormat(QImage::Format_RGB32);
    QImage denoised(input.size(), QImage::Format_RGB32);
    denoised.fill(Qt::black);

    const int width = input.width();
    const int height = input.height();
    for (int y = 0; y < height; ++y) {
        QRgb *outLine = reinterpret_cast<QRgb *>(denoised.scanLine(y));
        const QRgb *inLine = reinterpret_cast<const QRgb *>(input.constScanLine(y));
        for (int x = 0; x < width; ++x) {
            if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
                outLine[x] = inLine[x];
                continue;
            }

            std::array<int, 9> reds;
            std::array<int, 9> greens;
            std::array<int, 9> blues;
            int index = 0;
            for (int ky = -1; ky <= 1; ++ky) {
                const QRgb *scan = reinterpret_cast<const QRgb *>(input.constScanLine(y + ky));
                for (int kx = -1; kx <= 1; ++kx) {
                    const QRgb pixel = scan[x + kx];
                    reds[index] = qRed(pixel);
                    greens[index] = qGreen(pixel);
                    blues[index] = qBlue(pixel);
                    ++index;
                }
            }

            const QRgb center = inLine[x];
            const int mr = median9(reds);
            const int mg = median9(greens);
            const int mb = median9(blues);
            const int delta = qAbs(qRed(center) - mr) + qAbs(qGreen(center) - mg) + qAbs(qBlue(center) - mb);

            int r = qRed(center);
            int g = qGreen(center);
            int b = qBlue(center);
            if (delta > 34) {
                r = (r + mr * 4) / 5;
                g = (g + mg * 4) / 5;
                b = (b + mb * 4) / 5;
            } else {
                r = (r * 3 + mr) / 4;
                g = (g * 3 + mg) / 4;
                b = (b * 3 + mb) / 4;
            }
            outLine[x] = qRgb(r, g, b);
        }
    }

    QImage output(denoised.size(), QImage::Format_RGB32);
    output.fill(Qt::black);
    for (int y = 0; y < height; ++y) {
        QRgb *outLine = reinterpret_cast<QRgb *>(output.scanLine(y));
        const QRgb *srcLine = reinterpret_cast<const QRgb *>(denoised.constScanLine(y));
        for (int x = 0; x < width; ++x) {
            if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
                outLine[x] = srcLine[x];
                continue;
            }

            int blurR = 0;
            int blurG = 0;
            int blurB = 0;
            for (int ky = -1; ky <= 1; ++ky) {
                const QRgb *scan = reinterpret_cast<const QRgb *>(denoised.constScanLine(y + ky));
                for (int kx = -1; kx <= 1; ++kx) {
                    const QRgb pixel = scan[x + kx];
                    blurR += qRed(pixel);
                    blurG += qGreen(pixel);
                    blurB += qBlue(pixel);
                }
            }
            blurR /= 9;
            blurG /= 9;
            blurB /= 9;

            const QRgb center = srcLine[x];
            const int r = clampColor(qRed(center) + (qRed(center) - blurR) / 7);
            const int g = clampColor(qGreen(center) + (qGreen(center) - blurG) / 7);
            const int b = clampColor(qBlue(center) + (qBlue(center) - blurB) / 7);
            output.setPixel(x, y, qRgb(r, g, b));
        }
    }
    return output;
}

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
    m_saveDir = QDir(docs.isEmpty() ? QDir::homePath() : docs).filePath(QStringLiteral("ESP32Host"));
    m_imageDir = QDir(m_saveDir).filePath(QStringLiteral("images"));
    ensureSaveDirs();

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
QString HostController::statusText() const { return m_statusText; }
QString HostController::latestImageUrl() const { return m_latestImageUrl; }
QString HostController::latestFrameInfo() const { return m_latestFrameInfo; }
QString HostController::latestCategoryLabel() const { return m_latestCategoryLabel; }
int HostController::latestCategoryConfidence() const { return m_latestCategoryConfidence; }
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
QStringList HostController::logLines() const { return m_logLines; }
int HostController::brightness() const { return m_brightness; }
int HostController::motorSpeed() const { return m_motorSpeed; }
int HostController::dangerThreshold() const { return m_dangerThreshold; }
bool HostController::detectionEnabled() const { return m_detectionEnabled; }
bool HostController::previewOverlayEnabled() const { return m_previewOverlayEnabled; }

QVariantList HostController::dashboardCards() const
{
    QVariantList cards;
    cards.append(makeCard(QStringLiteral("连接状态"), connected() ? QStringLiteral("在线") : QStringLiteral("待连接"), m_statusText, connected() ? QStringLiteral("#49d39b") : QStringLiteral("#f2b84b")));
    cards.append(makeCard(QStringLiteral("接收吞吐"), QStringLiteral("%1 MB").arg(m_bytesReceived / 1048576.0, 0, 'f', 2), QStringLiteral("累计流量"), QStringLiteral("#54b8ff")));
    cards.append(makeCard(QStringLiteral("总包裹数"), QString::number(m_imageCount), QStringLiteral("按接收图像计数"), QStringLiteral("#8da2ff")));
    cards.append(makeCard(QStringLiteral("最新类别"), m_latestCategoryLabel, QStringLiteral("%1% 置信度").arg(m_latestCategoryConfidence), QStringLiteral("#ff7a90")));
    cards.append(makeCard(QStringLiteral("CPU0 / CPU1"), QStringLiteral("%1% / %2%").arg(m_cpu0Usage).arg(m_cpu1Usage), QStringLiteral("板端双核负载"), QStringLiteral("#49d39b")));
    cards.append(makeCard(QStringLiteral("图片耗时"), QStringLiteral("%1 / %2 ms").arg(m_lastImageEncodeMs).arg(m_lastImageSendMs), QStringLiteral("编码/发送"), QStringLiteral("#f2b84b")));
    cards.append(makeCard(QStringLiteral("运行时长"), m_uptimeText, QStringLiteral("下位机"), QStringLiteral("#54b8ff")));
    cards.append(makeCard(QStringLiteral("最大空闲块"), QStringLiteral("%1 MB").arg(m_largestBlockMb, 0, 'f', 2), QStringLiteral("内存碎片"), QStringLiteral("#8da2ff")));
    return cards;
}

QVariantList HostController::courierStatsCards() const
{
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

void HostController::setBrightness(int value) { updateControl(QStringLiteral("brightness"), qBound(0, value, 100), true); }
void HostController::setMotorSpeed(int value) { updateControl(QStringLiteral("motor_speed"), qBound(0, value, 100), true); }
void HostController::setDangerThreshold(int value) { updateControl(QStringLiteral("danger_threshold"), qBound(0, value, 100), true); }
void HostController::setDetectionEnabled(bool enabled) { updateControl(QStringLiteral("detection_enabled"), enabled, true); }
void HostController::setPreviewOverlayEnabled(bool enabled) { updateControl(QStringLiteral("preview_overlay"), enabled, true); }

void HostController::sendControl(const QString &command, const QVariant &value)
{
    updateControl(command, value, false);
}

void HostController::clearFrameHistory()
{
    if (m_frameHistory.isEmpty()) {
        return;
    }
    m_frameHistory.clear();
    emit detectionChanged();
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

void HostController::onNetworkImageResultReady(quint16 protocolVersion, quint32 frameId, quint16 width, quint16 height,
                                               quint16 classId, quint16 confidenceX1000, quint16 inferTimeMs,
                                               const QVariantList &boxes, const QByteArray &jpeg)
{
    const int confidencePct = qBound(0, qRound(double(confidenceX1000) / 10.0), 100);
    const QVariantList detections = makeImageDetections(width, height, boxes);
    if (!saveLatestPreviewImage(frameId, width, height, HostProtocol::kPixelJpeg, jpeg)) {
        appendLog(QStringLiteral("V%1 JPEG 解码失败 frame=%2").arg(protocolVersion).arg(frameId));
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

bool HostController::saveLatestPreviewImage(quint32 frameSeq, quint16 width, quint16 height, quint16 pixelFormat, const QByteArray &imagePayload)
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

    // 下位机 BGR888 数据被当作 RGB888 送入 JPEG 编码，红蓝通道互换，此处交换回来
    decoded = decoded.rgbSwapped();

    const QImage enhanced = enhancePreviewImage(decoded);
    if (enhanced.isNull()) {
        return false;
    }
    const QByteArray bytes = encodeJpeg(enhanced, 88);
    if (bytes.isEmpty()) {
        return false;
    }

    const QString framePath = QDir(m_imageDir).filePath(QStringLiteral("frame_%1.jpg").arg(frameSeq, 6, 10, QLatin1Char('0')));
    const QString latestPath = QDir(m_imageDir).filePath(QStringLiteral("latest_preview.jpg"));
    if (!writeBytesAtomic(framePath, bytes) || !writeBytesAtomic(latestPath, bytes)) {
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
            detection.insert(QStringLiteral("label"), stage == 0 ? QStringLiteral("面单") : categoryLabelFromId(box.category));
            detection.insert(QStringLiteral("color"), stage == 0 ? QStringLiteral("#00ff00")
                                                                   : box.category == 1 ? QStringLiteral("#ffff00")
                                                                   : box.category == 2 ? QStringLiteral("#0066ff")
                                                                                       : QStringLiteral("#ff3030"));
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

    QVariantMap record;
    record.insert(QStringLiteral("seq"), int(frameSeq));
    record.insert(QStringLiteral("time"), QDateTime::currentDateTime().toString(QStringLiteral("HH:mm:ss")));
    record.insert(QStringLiteral("imageUrl"), imageUrl);
    const QString packageLabel = QStringLiteral("包裹#%1").arg(++m_packageDisplaySeq);
    record.insert(QStringLiteral("packageLabel"), packageLabel);
    record.insert(QStringLiteral("categoryLabel"), category);
    record.insert(QStringLiteral("categoryConfidence"), confidence);
    record.insert(QStringLiteral("title"), QStringLiteral("%1  %2").arg(packageLabel, category));
    record.insert(QStringLiteral("model"), QStringLiteral("板端 %1 预览").arg(formatText));
    record.insert(QStringLiteral("resolution"), QStringLiteral("%1 x %2").arg(width).arg(height));
    record.insert(QStringLiteral("processMs"), inferTimeMs);
    record.insert(QStringLiteral("count"), detections.size());
    record.insert(QStringLiteral("confidence"), confidence / 100.0);
    record.insert(QStringLiteral("danger"), confidence < m_dangerThreshold);
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
    } else if (command == QStringLiteral("detection_enabled")) {
        const bool next = value.toBool();
        changed = m_detectionEnabled != next;
        m_detectionEnabled = next;
    } else if (command == QStringLiteral("preview_overlay")) {
        const bool next = value.toBool();
        changed = m_previewOverlayEnabled != next;
        m_previewOverlayEnabled = next;
    } else {
        changed = true;
    }

    if (command == QStringLiteral("motor_speed")) {
        queueControlSend(command, value);
    }
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
    QString line;
    if (command == QStringLiteral("motor_speed")) {
        const int speed = qBound(0, value.toInt(), 100);
        line = QStringLiteral("CONFIG a_speed=%1 b_speed=%1 c_speed=%1").arg(speed);
    } else {
        appendLog(QStringLiteral("本地控制：%1 = %2").arg(command, value.toString()));
        return;
    }

    sendSimLine(line);
    appendLog(QStringLiteral("%1命令：%2")
                  .arg(connected() ? QStringLiteral("已发送") : QStringLiteral("待连接记录"))
                  .arg(line));
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
