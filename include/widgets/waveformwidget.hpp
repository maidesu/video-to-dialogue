#pragma once

#include <QLineSeries>
#include <QChart>

namespace DialogueFromVideo {

class WaveformWidget : public QChart
{
    Q_OBJECT
public:
    explicit WaveformWidget();

public slots:

private:
    int m_sampleRate;
    QLineSeries* m_series;

signals:

};

} // namespace DialogueFromVideo
