#include "demodatasource.h"

#include <QDateTime>
#include <QTimer>
#include <QtMath>

DemoDataSource::DemoDataSource(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this))
{
    m_timer->setInterval(900);
    connect(m_timer, &QTimer::timeout, this, &DemoDataSource::tick);
    m_timer->start();
}

void DemoDataSource::tick()
{
    ++m_tick;
    const double wave = qSin(m_tick / 4.0);
    const double fast = qSin(m_tick / 1.8);

    QVariantMap metrics;
    metrics.insert(QStringLiteral("cpu_usage"), qBound(8, int(54 + wave * 22 + fast * 6), 96));
    metrics.insert(QStringLiteral("free_heap_mb"), 8.5 + qSin(m_tick / 5.0) * 1.8);
    metrics.insert(QStringLiteral("free_internal_mb"), 0.38 + qSin(m_tick / 6.0) * 0.08);
    metrics.insert(QStringLiteral("free_psram_mb"), 5.8 + qSin(m_tick / 7.0) * 1.1);
    metrics.insert(QStringLiteral("total_psram_mb"), 8.0);
    metrics.insert(QStringLiteral("largest_block_mb"), 2.1 + qSin(m_tick / 3.0) * 0.35);
    metrics.insert(QStringLiteral("latency_ms"), qBound(12, int(38 + fast * 14), 82));
    metrics.insert(QStringLiteral("fps"), 10.0 + qSin(m_tick / 5.0) * 2.5);
    metrics.insert(QStringLiteral("uptime_ms"), qint64(1000) * (3600 + m_tick * 2));
    emit metricsReady(metrics);

    if (m_tick % 4 == 0) {
        QVariantMap frame;
        frame.insert(QStringLiteral("frame_seq"), ++m_frameSeq);
        frame.insert(QStringLiteral("timestamp_ms"), QDateTime::currentMSecsSinceEpoch());
        frame.insert(QStringLiteral("model"), QStringLiteral("YOLO-ExpressLogo INT8 Demo"));
        frame.insert(QStringLiteral("image_width"), 640);
        frame.insert(QStringLiteral("image_height"), 384);
        frame.insert(QStringLiteral("inference_ms"), 21 + (m_tick % 7));
        frame.insert(QStringLiteral("detections"), makeDetections(m_frameSeq));
        emit detectionReady(frame);
    }
}

QVariantList DemoDataSource::makeDetections(int frameSeq) const
{
    const QStringList labels = {
        QStringLiteral("顺丰速运"),
        QStringLiteral("京东物流"),
        QStringLiteral("中通快递"),
        QStringLiteral("圆通速递"),
        QStringLiteral("韵达快递")
    };

    QVariantList detections;
    const int count = 2 + frameSeq % 3;
    for (int i = 0; i < count; ++i) {
        QVariantMap item;
        const int confidence = (i == count - 1 && frameSeq % 2 == 0) ? 43 : 62 + ((frameSeq + i * 13) % 32);
        item.insert(QStringLiteral("label"), labels.at((frameSeq + i) % labels.size()));
        item.insert(QStringLiteral("confidence"), confidence / 100.0);
        item.insert(QStringLiteral("x"), 0.12 + 0.19 * i);
        item.insert(QStringLiteral("y"), 0.16 + 0.11 * ((frameSeq + i) % 3));
        item.insert(QStringLiteral("w"), 0.22);
        item.insert(QStringLiteral("h"), 0.18);
        item.insert(QStringLiteral("track_id"), 100 + i);
        detections.append(item);
    }
    return detections;
}
