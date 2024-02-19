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

    void fill(const QVector<double>& samples);/*,
              int sampleRate,
              QList<Interval>& dialogue,
              QList<Interval>& subtitle,
              QList<Interval>& padding,
              QList<Interval>& gap);*/

    void reset();

public slots:

private:
    int m_sampleRate;
    QVector<QPointF> m_buff;
    QLineSeries* m_series;
    QValueAxis* m_xaxis;
    QValueAxis* m_yaxis;

signals:

};

} // namespace DialogueFromVideo
