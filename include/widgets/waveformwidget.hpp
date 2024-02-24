#pragma once

#include <common/subtitlestructs.hpp>

#include <QLineSeries>
#include <QAreaSeries>
#include <QValueAxis>
#include <QChart>

namespace DialogueFromVideo {

class WaveformWidget : public QChart
{
    Q_OBJECT
public:
    explicit WaveformWidget();

    virtual ~WaveformWidget();

    void initialize(int sampleCount,
                    int sampleRate);

    void plotWaveform(const QVector<double>& samples);

    void drawIntervals(const QList<Interval>& dialogue,
                       const QList<Interval>& subtitle,
                       const QList<Interval>& padding,
                       const QList<Interval>& gap);

    void reset();

    const int m_target = 100000; // Total samples picked

    const QList<QColor> m_intervalColors { QColor(51, 204, 51, 128),
                                           QColor(102, 153, 153, 64),
                                           QColor(255, 173, 51, 96),
                                           QColor(238, 0, 238, 64) };

private:
    int m_sampleRate;
    int m_sampleCount;

    QLineSeries* m_series;

    QValueAxis* m_xaxis;
    QValueAxis* m_yaxis;

    QLineSeries* m_dialogueUpperSeries;
    QLineSeries* m_dialogueLowerSeries;
    QAreaSeries* m_dialogueSeries;

    QLineSeries* m_subtitleUpperSeries;
    QLineSeries* m_subtitleLowerSeries;
    QAreaSeries* m_subtitleSeries;

    QLineSeries* m_paddingUpperSeries;
    QLineSeries* m_paddingLowerSeries;
    QAreaSeries* m_paddingSeries;

    QLineSeries* m_gapUpperSeries;
    QLineSeries* m_gapLowerSeries;
    QAreaSeries* m_gapSeries;
};

} // namespace DialogueFromVideo
