#include <dialogue/dialogue.hpp>

namespace DialogueFromVideo {

Dialogue::Dialogue(QObject* parent)
    : QObject(parent)
{
}

void Dialogue::processSubs(const QList<SubEntry *>& subs)
{
    (void)subs;
}

void Dialogue::cleanUp()
{
    m_dialogue.clear();
    m_subtitle.clear();
    m_padding.clear();
    m_gap.clear();
}

Dialogue::~Dialogue()
{
}

} // namespace DialogueFromVideo
