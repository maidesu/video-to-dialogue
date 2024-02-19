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

public slots:
    void processDialogueHandler(const QList<SubEntry*>& subs,
                                int64_t left,
                                int64_t right,
                                int64_t offset,
                                int64_t minGap);

    void clearDialogueHandler();

private:
    void processDialogue(const QList<SubEntry*>& subs,
                         int64_t left,
                         int64_t right,
                         int64_t offset,
                         int64_t minGap);

    void clearDialogue();

    QList<Interval> m_dialogue; // Timestamps to be used for extraction
    QList<Interval> m_subtitle; // All lists including this are offset
    QList<Interval> m_padding;
    QList<Interval> m_gap;

    Messenger m_messenger;
    Progress m_progress;

signals:
    void readyDialogueSignal(const QList<Interval>& dialogue,
                             const QList<Interval>& subtitle,
                             const QList<Interval>& padding,
                             const QList<Interval>& gap);

};

} // namespace DialogueFromVideo
