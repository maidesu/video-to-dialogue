#include <dialogue/dialogue.hpp>

namespace DialogueFromVideo {

Dialogue::Dialogue(QObject* parent)
    : QObject(parent)
{
}

void Dialogue::processDialogueHandler(const QList<SubEntry*>& subs,
                                      int64_t left,
                                      int64_t right,
                                      int64_t offset,
                                      int64_t minGap)
{
    processDialogue(subs, left, right, offset, minGap);

    emit m_messenger.print(tr("Extracted dialogue"),
                           "Dialogue",
                           MessageLevel::Info);

    emit readyDialogueSignal(m_dialogue,
                             m_subtitle,
                             m_padding,
                             m_gap);
}

void Dialogue::clearDialogueHandler()
{
    clearDialogue();
}

void Dialogue::processDialogue(const QList<SubEntry*>& subs,
                               int64_t left,
                               int64_t right,
                               int64_t offset,
                               int64_t minGap) // minimum required for a separation
{
    /*
     * We won't validate whether the results are
     * still in the range of a given audio track.
     * That task is reserved for the exporting party.
     */

    clearDialogue();

    if (subs.empty())
    {
        return;
    }


    // Used for m_dialogue and m_gap
    int64_t start = subs[0]->start + offset - left;
    int64_t end = subs[0]->end + offset + right;

    // Subtitle with offset
    m_subtitle.append({ subs[0]->start + offset, subs[0]->end + offset });

    // Padding with offset on each side
    m_padding.append({ start, subs[0]->start + offset });
    m_padding.append({ subs[0]->end + offset, end });

    for (int i = 1; i < subs.size(); ++i)
    {
        const SubEntry* next_sub = subs[i];

        // Used for m_dialogue and m_gap
        int64_t next_start = next_sub->start + offset - left;
        int64_t next_end = next_sub->end + offset + right;

        m_subtitle.append({ next_sub->start + offset, next_sub->end + offset });
        m_padding.append({ next_start, next_sub->start + offset });
        m_padding.append({ next_sub->end + offset, next_end });

        /*
         * Condition where dialogue as well as gap is invalidated
         * also known as negative length interval
         */
        if (next_start >= next_end)
        {
            continue;
        }

        /*
         * The gap is larger or equal to the minimum
         * gap required for separation.
         * Therefore, we separate.
         */
        if (end + minGap <= next_start)
        {
            if (start < end) // The 0th element is not validated
            {
                m_dialogue.append({ start, end });
            }

            start = next_start;
            end = next_end;
        }
        /*
         * Here the gap is smaller,
         * we want to merge
         */
        else
        {
            if (start < end) // The 0th element is not validated
            {
                m_gap.append({ end, next_start });
            }
            else
            {
                start = next_start;
            }

            end = next_end;
        }
    }

    if (start < end) // The 0th element is not validated
    {
        m_dialogue.append({ start, end });
    }
}

Dialogue::~Dialogue()
{
}

void Dialogue::clearDialogue()
{
    m_dialogue.clear();
    m_subtitle.clear();
    m_padding.clear();
    m_gap.clear();
}

} // namespace DialogueFromVideo
