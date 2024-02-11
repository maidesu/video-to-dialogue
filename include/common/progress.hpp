#pragma once

#include <common/progressbar.hpp>

#include <QObject>

namespace DialogueFromVideo {

class Progress : public QObject
{
    Q_OBJECT
public:
    explicit Progress(QObject *parent = nullptr)
        : QObject(parent)
    {
        QObject::connect(this,
                         &Progress::progressAdd,
                         &ProgressBar::instance(),
                         &ProgressBar::progressAddHandler);

        QObject::connect(this,
                         &Progress::progressMaximum,
                         &ProgressBar::instance(),
                         &ProgressBar::progressMaximumHandler);

        QObject::connect(this,
                         &Progress::progressComplete,
                         &ProgressBar::instance(),
                         &ProgressBar::progressCompleteHandler);

        QObject::connect(this,
                         &Progress::progressReset,
                         &ProgressBar::instance(),
                         &ProgressBar::progressResetHandler);
    }

    virtual ~Progress() {}

signals:
    void progressAdd(int val);
    void progressMaximum(int max);
    void progressComplete();
    void progressReset();
};

} // namespace DialogueFromVideo
