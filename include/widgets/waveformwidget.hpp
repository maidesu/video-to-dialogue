#pragma once

#include <common/subtitlestructs.hpp>

#include <QLineSeries>
#include <QValueAxis>
#include <QChart>

namespace DialogueFromVideo {

class WaveformWidget : public QChart
{
    Q_OBJECT
public:
    explicit WaveformWidget();

    virtual ~WaveformWidget();

    void plotWaveform(const QVector<double>& samples);

    void drawIntervals(const QList<Interval>& dialogue,
                       const QList<Interval>& subtitle,
                       const QList<Interval>& padding,
                       const QList<Interval>& gap,
                       int sampleRate);

    void reset();

    const QList<QColor> m_intervalColors { Qt::green,
                                           Qt::magenta,
                                           Qt::cyan,
                                           Qt::blue };

private:
    int m_sampleRate;
    QVector<QPointF> m_buff;
    QLineSeries* m_series;
    QValueAxis* m_xaxis;
    QValueAxis* m_yaxis;

};

} // namespace DialogueFromVideo
