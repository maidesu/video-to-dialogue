#include <subtitle/subtitle.hpp>

namespace DialogueFromVideo {

Subtitle::~Subtitle()
{
    for (SubEntry* se : m_subs)
    {
        delete se;
    }
}

void Subtitle::extractSubtitle(File::Read* file,
                               int selectedSubIndex,
                               int selectedSubLayerIndex)
{
    AVStream* subStream = file->getStream(selectedSubIndex);

    (void)subStream;
    (void)selectedSubLayerIndex;
}

} // namespace DialogueFromVideo
