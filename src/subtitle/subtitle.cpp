#include <subtitle/subtitle.hpp>

namespace DialogueFromVideo {

Subtitle::Subtitle(QObject* parent)
    : Messenger(parent)
{
}

Subtitle::~Subtitle()
{
    for (SubEntry* se : m_subs)
    {
        delete se;
    }
}

void Subtitle::subtitleRequestedHandler(File::Read* file,
                                        int selectedSubIndex,
                                        int selectedSubLayerIndex)
{
    clearSubs();

    AVStream* subStream = file->getStream(selectedSubIndex);

    (void)subStream;
    (void)selectedSubLayerIndex;

    emit Subtitle::subtitleExtractedSignal(m_subs);
}

void Subtitle::clearSubs()
{
    for (SubEntry* se : m_subs)
    {
        delete se;
    }

    m_subs.clear();
}

} // namespace DialogueFromVideo
