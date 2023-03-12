#include "fileinfo.hpp"

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
}

namespace DialogueFromVideo {

QString FileInfo::openFilePath()
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

void FileInfo::getAudioCodecs()
{
    const char    *url = "in.mp3";
    AVFormatContext *s = NULL;
    int ret = avformat_open_input(&s, url, NULL, NULL);
    if (ret < 0)
        abort();
}

} // namespace DialogueFromVideo
