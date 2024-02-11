#pragma once

#include <common/singleton.hpp>
#include <common/messenger.hpp>

#include <QProgressBar>

namespace DialogueFromVideo {

class ProgressBar : public QObject, public Singleton<ProgressBar>
{
    Q_OBJECT

    friend class Singleton<ProgressBar>;

public:
    QProgressBar* progressBar() const { return m_progressBar; }

public slots:
    void progressAddHandler(int val)
    {
        m_progressBar->setValue(m_progressBar->value() + val);

        emit m_messenger.print(QString("New value is %1").arg(m_progressBar->value()),
                               "ProgressBar",
                               MessageLevel::Debug);
    }

    void progressMaximumHandler(int max)
    {
        m_progressBar->setMaximum(max);

        emit m_messenger.print(QString("Setting max to %1").arg(max),
                               "ProgressBar",
                               MessageLevel::Debug);
    }

    void progressCompleteHandler()
    {
        m_progressBar->setValue(m_progressBar->maximum());

        emit m_messenger.print("Bar has been completed",
                               "ProgressBar",
                               MessageLevel::Debug);
    }

    void progressResetHandler()
    {
        m_progressBar->reset();

        emit m_messenger.print("Bar has been reset",
                               "ProgressBar",
                               MessageLevel::Debug);
    }

private:
    ProgressBar(QObject *parent = nullptr)
        : QObject(parent)
        , m_progressBar(new QProgressBar())
    {
    }

    QProgressBar* m_progressBar;

    Messenger m_messenger;
};

} // namespace DialogueFromVideo
