#include "fileinfo.hpp"

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
}

namespace DialogueFromVideo {

void FileInfo::openFile()
{
    QString openPath =
            QFileDialog::getOpenFileName(nullptr,
                                         tr("Open File"),
                                         "",
                                         tr("Video Files (*.mp4 *.m4a *.m4v *.mov *.avi *.mkv *.flv *.wmv *.qt *.ts *.mpg *.mpeg *.ogv);;All Files (*)"));

    if (!openPath.isEmpty())
    {
        path = openPath.toUtf8().constData();
        emit print(QString("Selected file: %1").arg(path), "FileInfo", MessageLevel::Info);

        getFileInfoFfmpeg();
    }
    else
    {
        emit print("Received an invalid file path!", "FileInfo", MessageLevel::Error);
    }
}

void FileInfo::getFileInfoFfmpeg()
{
    AVFormatContext* formatContext = NULL;

    int res = avformat_open_input(&formatContext, path, NULL, NULL);
    if (res < 0)
    {
        avformat_close_input(&formatContext);
        emit print("Failed to open file path!", "FileInfo", MessageLevel::Error);

        return;
    }
    else
    {
        emit print("Successfully opened file!", "FileInfo", MessageLevel::Info);
    }

    AVStream* stream;
    for (uint i = 0; i < formatContext->nb_streams; ++i) {
        stream = formatContext->streams[i];

        if (stream->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE) {
            subStreams.append(QString::number(i));
        }
        else if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStreams.append(QString::number(i));
        }
    }

    avformat_close_input(&formatContext);

    emit fileChanged(subStreams, audioStreams);
}

} // namespace DialogueFromVideo
