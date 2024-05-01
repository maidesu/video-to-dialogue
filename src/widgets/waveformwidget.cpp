#include <widgets/waveformwidget.hpp>

#include <QtMath>

namespace DialogueFromVideo {

WaveformWidget::WaveformWidget()
    : QChart()
    , m_sampleRate(0)
    , m_sampleCount(0)
    , m_series(new QLineSeries())
    , m_xaxis(new QValueAxis())
    , m_yaxis(new QValueAxis())
    , m_dialogueUpperSeries(new QLineSeries())
    , m_dialogueLowerSeries(new QLineSeries())
    , m_dialogueSeries(new QAreaSeries(m_dialogueUpperSeries, m_dialogueLowerSeries))
    , m_subtitleUpperSeries(new QLineSeries())
    , m_subtitleLowerSeries(new QLineSeries())
    , m_subtitleSeries(new QAreaSeries(m_subtitleUpperSeries, m_subtitleLowerSeries))
    , m_paddingUpperSeries(new QLineSeries())
    , m_paddingLowerSeries(new QLineSeries())
    , m_paddingSeries(new QAreaSeries(m_paddingUpperSeries, m_paddingLowerSeries))
    , m_gapUpperSeries(new QLineSeries())
    , m_gapLowerSeries(new QLineSeries())
    , m_gapSeries(new QAreaSeries(m_gapUpperSeries, m_gapLowerSeries))
{
    this->addSeries(m_series);
    this->addSeries(m_dialogueSeries);
    this->addSeries(m_subtitleSeries);
    this->addSeries(m_paddingSeries);
    this->addSeries(m_gapSeries);


    m_series->setName(tr("Waveform"));
    m_dialogueSeries->setName(tr("Dialogue"));
    m_subtitleSeries->setName(tr("Subtitle"));
    m_paddingSeries->setName(tr("Padding"));
    m_gapSeries->setName(tr("Gap"));


    m_xaxis->setTitleText(tr("Samples"));
    m_xaxis->setTickCount(2);
    m_xaxis->setLabelFormat("%n"); // printf nothing printed
    m_xaxis->setRange(1, m_target);

    m_yaxis->setTitleText(tr("Level"));
    m_yaxis->setTickCount(2);
    m_yaxis->setLabelFormat("%n");

    this->addAxis(m_xaxis, Qt::AlignBottom);
    this->addAxis(m_yaxis, Qt::AlignLeft);


    m_series->attachAxis(m_xaxis);
    m_series->attachAxis(m_yaxis);

    m_dialogueSeries->attachAxis(m_xaxis);
    m_dialogueSeries->attachAxis(m_yaxis);

    m_subtitleSeries->attachAxis(m_xaxis);
    m_subtitleSeries->attachAxis(m_yaxis);

    m_paddingSeries->attachAxis(m_xaxis);
    m_paddingSeries->attachAxis(m_yaxis);

    m_gapSeries->attachAxis(m_xaxis);
    m_gapSeries->attachAxis(m_yaxis);


    this->legend()->hide();
}

void WaveformWidget::initialize(int sampleCount, int sampleRate)
{
    m_sampleRate = sampleRate;
    m_sampleCount = sampleCount;
}

void WaveformWidget::plotWaveform(const QVector<double>& samples)
{
    if (m_sampleCount < m_target)
    {
        return;
    }

    QVector<QPointF> buff;

    buff.reserve(m_target);

    int divisor = m_sampleCount / m_target;

    for (int j = 1, i = 0; i < samples.size(); ++i)
    {
        if (i % divisor == 0)
        {
            buff.append(QPointF(j++, samples[i])); // Cherry picking
        }
    }

    // Abs max
    qreal max = 0;
    for (QPointF pt : buff)
    {
        qreal val = qFabs(pt.y());

        if (val > max)
        {
            max = val;
        }
    }

    m_yaxis->setRange(-max, max); // Relative y scale

    m_series->replace(buff);
}

void WaveformWidget::drawIntervals(const QList<Interval>& dialogue,
                                   const QList<Interval>& subtitle,
                                   const QList<Interval>& padding,
                                   const QList<Interval>& gap)
{
    // Have to set this here due to variable lifetime
    m_dialogueSeries->setPen(Qt::NoPen);
    m_dialogueSeries->setColor(m_intervalColors[0]);

    m_subtitleSeries->setPen(Qt::NoPen);
    m_subtitleSeries->setColor(m_intervalColors[1]);

    m_paddingSeries->setPen(Qt::NoPen);
    m_paddingSeries->setColor(m_intervalColors[2]);

    m_gapSeries->setPen(Qt::NoPen);
    m_gapSeries->setColor(m_intervalColors[3]);

    this->legend()->show();


    if (m_sampleCount < m_target || !m_sampleRate)
    {
        return;
    }

    double duration = static_cast<int64_t>(m_sampleCount) * 1000 / static_cast<double>(m_sampleRate);

    QVector<QPointF> upperBuff;
    QVector<QPointF> lowerBuff;


    auto drawRects = [&](const QList<Interval>& intervals,
                         QLineSeries* upperSeries,
                         QLineSeries* lowerSeries,
                         double height)
    {
        upperBuff.clear();
        lowerBuff.clear();

        if (intervals.isEmpty())
        {
            return;
        }

        upperBuff.append({ (intervals[0].start * m_target) / duration, height });
        upperBuff.append({ (intervals[0].end * m_target) / duration, height });

        lowerBuff.append({ (intervals[0].start * m_target) / duration, -height });
        lowerBuff.append({ (intervals[0].end * m_target) / duration, -height });

        for (int i = 1; i < intervals.size(); ++i)
        {
            upperBuff.append({ (intervals[i-1].end * m_target) / duration, 0 });
            upperBuff.append({ (intervals[i].start * m_target) / duration, 0 });

            lowerBuff.append({ (intervals[i-1].end * m_target) / duration, 0 });
            lowerBuff.append({ (intervals[i].start * m_target) / duration, 0 });

            upperBuff.append({ (intervals[i].start * m_target) / duration, height });
            upperBuff.append({ (intervals[i].end * m_target) / duration, height });

            lowerBuff.append({ (intervals[i].start * m_target) / duration, -height });
            lowerBuff.append({ (intervals[i].end * m_target) / duration, -height });
        }

        upperSeries->replace(upperBuff);
        lowerSeries->replace(lowerBuff);
    };


    drawRects(subtitle, m_subtitleUpperSeries, m_subtitleLowerSeries, m_yaxis->max());
    drawRects(padding, m_paddingUpperSeries, m_paddingLowerSeries, m_yaxis->max() / 4 * 3);
    drawRects(gap, m_gapUpperSeries, m_gapLowerSeries, m_yaxis->max() / 2);
    drawRects(dialogue, m_dialogueUpperSeries, m_dialogueLowerSeries, m_yaxis->max() / 4);
}

void WaveformWidget::reset()
{
    m_gapLowerSeries->clear();
    m_gapUpperSeries->clear();

    m_paddingLowerSeries->clear();
    m_paddingUpperSeries->clear();

    m_subtitleLowerSeries->clear();
    m_subtitleUpperSeries->clear();

    m_dialogueLowerSeries->clear();
    m_dialogueUpperSeries->clear();

    m_series->clear();

    this->legend()->hide();

    m_sampleCount = 0;
    m_sampleRate = 0;
}

WaveformWidget::~WaveformWidget()
{
    delete m_gapLowerSeries;
    delete m_gapUpperSeries;
    delete m_paddingLowerSeries;
    delete m_paddingUpperSeries;
    delete m_subtitleLowerSeries;
    delete m_subtitleUpperSeries;
    delete m_dialogueLowerSeries;
    delete m_dialogueUpperSeries;
    delete m_series;
}

} // namespace DialogueFromVideo
