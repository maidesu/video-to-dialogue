#include "file.hpp"

extern "C" {
    #include "libavcodec/avcodec.h"
}

namespace DialogueFromVideo {

QString File::openFilePath()
{
    QString openPath =
            QFileDialog::getOpenFileName(nullptr,
                                         tr("Open File"),
                                         "",
                                         tr("Video Files (*.mp4 *.m4a *.m4v *.mov *.avi *.mkv *.flv *.wmv *.qt *.ts *.mpg *.mpeg *.ogv);;All Files (*)"));

    if (!openPath.isEmpty())
    {
        path = openPath;
    }

    return path;
}

void File::getAudioCodecs()
{
    av_register_all();
    avcodec_register_all();
}

} // namespace DialogueFromVideo
