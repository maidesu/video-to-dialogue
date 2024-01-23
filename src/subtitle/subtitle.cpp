#include "../include/subtitle.hpp"

#include "../include/file/open.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

namespace DialogueFromVideo {

Subtitle::Subtitle(QObject *parent)
    : Messenger(parent)
{

}

void Subtitle::extract(const char* path,
                       int selectedSubIndex,
                       int selectedSubLayerIndex)
{
    File::Open file(path);

    if (file.getResult() < 0)
    {
        emit print(tr("Failed to open file path!"), "Subtitle", MessageLevel::Error);

        return;
    }

    if (file.getResult() == 1) {
        emit print(tr("Failed to fill in missing stream information!"), "Subtitle", MessageLevel::Warning);
    }

    // Find decoder
    const AVCodec* decoder = avcodec_find_decoder(file.getStream(selectedSubIndex)->codecpar->codec_id);

    if (!decoder) {
        emit print(tr("Failed to find decoder!"), "Subtitle", MessageLevel::Error);

        return;
    }

    (void)selectedSubLayerIndex;
}

} // namespace DialogueFromVideo
