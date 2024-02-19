#include <widgets/waveformwidget.hpp>

#include <QtMath>

namespace DialogueFromVideo {

WaveformWidget::WaveformWidget()
    : QChart()
    , m_sampleRate(0)
    , m_series(new QLineSeries())
    , m_xaxis(new QValueAxis())
    , m_yaxis(new QValueAxis())
{
    this->addSeries(m_series);

    m_xaxis->setTitleText("Samples");
    //m_xaxis->setTickType(QValueAxis::TickType::TicksFixed);
    m_xaxis->setTickCount(2);
    m_xaxis->setLabelFormat("%n"); // prinf nothing printed

    m_yaxis->setTitleText("Level");
    //m_yaxis->setTickType(QValueAxis::TickType::TicksFixed);
    m_yaxis->setTickCount(2);
    m_yaxis->setLabelFormat("%n");

    this->addAxis(m_xaxis, Qt::AlignBottom);
    m_series->attachAxis(m_xaxis);
    this->addAxis(m_yaxis, Qt::AlignLeft);
    m_series->attachAxis(m_yaxis);

    this->legend()->hide();
}

void WaveformWidget::plotWaveform(const QVector<double>& samples)
{
    constexpr int target = 100000; // Total samples picked

    if (samples.size() < target)
    {
        return;
    }

    m_buff.reserve(target);

    int divisor = samples.size() / target;

    for (int j = 1, i = 0; i < samples.size(); ++i)
    {
        if (i % divisor == 0)
        {
            m_buff.append(QPointF(j++, samples[i])); // Cherry picking
        }
    }

    // Abs max
    qreal max = 0;
    for (QPointF pt : m_buff)
    {
        qreal val = qFabs(pt.y());

        if (val > max)
        {
            max = val;
        }
    }

    m_xaxis->setRange(1, target);
    m_yaxis->setRange(-max, max); // Relative y scale

    m_series->replace(m_buff);
}

void WaveformWidget::drawIntervals(const QList<Interval>& dialogue,
                                   const QList<Interval>& subtitle,
                                   const QList<Interval>& padding,
                                   const QList<Interval>& gap,
                                   int sampleRate)
{
    (void)dialogue;
    (void)subtitle;
    (void)padding;
    (void)gap;
    (void)sampleRate;


}

void WaveformWidget::reset()
{
    m_series->clear();
    m_buff.clear();
    m_sampleRate = 0;
}

WaveformWidget::~WaveformWidget()
{
    delete m_series;
}

} // namespace DialogueFromVideo
