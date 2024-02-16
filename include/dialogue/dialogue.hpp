#pragma once

#include <file/read.hpp>
#include <common/subtitlestructs.hpp>
#include <common/messenger.hpp>
#include <common/progress.hpp>

#include <QList>

namespace DialogueFromVideo {

class Dialogue : public QObject
{
    Q_OBJECT
public:
    explicit Dialogue(QObject* parent = nullptr);
    virtual ~Dialogue();

    Dialogue(const Dialogue&) = delete;
    Dialogue& operator= (const Dialogue&) = delete;

    void processSubs(const QList<SubEntry*>& subs);

public slots:


private:
    void cleanUp();

    QList<Interval> m_dialogue; // Timestamps to be used for extraction
    QList<Interval> m_subtitle; // All lists including m_subtitle are offset
    QList<Interval> m_padding;
    QList<Interval> m_gap;

    Messenger m_messenger;
    Progress m_progress;

signals:

};

} // namespace DialogueFromVideo
