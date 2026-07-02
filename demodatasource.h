#pragma once

#include <QObject>
#include <QVariantList>

class QTimer;

class DemoDataSource : public QObject
{
    Q_OBJECT

public:
    explicit DemoDataSource(QObject *parent = nullptr);

signals:
    void metricsReady(const QVariantMap &metrics);
    void detectionReady(const QVariantMap &frame);

private:
    void tick();
    QVariantList makeDetections(int frameSeq) const;

    QTimer *m_timer = nullptr;
    int m_tick = 0;
    int m_frameSeq = 9000;
};
