#include "../include/fileinfo.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

namespace DialogueFromVideo {

FileInfo::FileInfo(QObject *parent)
    : Messenger(parent)
{

}

FileInfo::~FileInfo()
{
    for (SubInfo* si : m_subStreams)
    {
        delete si;
    }
    for (AudioInfo* ai : m_audioStreams)
    {
        delete ai;
    }
}

bool FileInfo::openFile()
{
    QString openPath =
            QFileDialog::getOpenFileName(nullptr,
                                         tr("Open File"),
                                         "",
                                         tr("Video Files (*.mp4 *.m4a *.m4v *.mov *.avi *.mkv *.flv *.wmv *.qt *.ts *.mpg *.mpeg *.ogv);;All Files (*)"));

    if (!openPath.isEmpty())
    {
        QByteArray bytes = openPath.toUtf8();
        m_path = bytes.constData();

        emit print(tr("Selected file: %1").arg(openPath), "FileInfo", MessageLevel::Info);
        emit print(tr("Selected path: %1").arg(m_path), "FileInfo", MessageLevel::Debug);

        return getFileInfoFfmpeg();
    }
    else
    {
        emit print(tr("Received no file path!"), "FileInfo", MessageLevel::Warning);

        return false;
    }
}

bool FileInfo::getFileInfoFfmpeg()
{
    AVFormatContext* formatContext = NULL;

    int res = avformat_open_input(&formatContext, m_path, NULL, NULL);
    if (res < 0)
    {
        avformat_close_input(&formatContext);
        emit print(tr("Failed to open file path!"), "FileInfo", MessageLevel::Error);

        return false;
    }

    emit print(tr("Successfully opened file!"), "FileInfo", MessageLevel::Info);

    // At this point we drop all content from before
    m_subStreams.clear();
    m_audioStreams.clear();

    AVStream* stream;
    for (uint i = 0; i < formatContext->nb_streams; ++i) {
        stream = formatContext->streams[i];

        if (stream->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE) {
            SubInfo* si = new SubInfo();

            si->index = stream->id;
            //si->lang = stream->codec->

            m_subStreams.append(si);
        }
        else if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            AudioInfo* ai = new AudioInfo();

            ai->index = stream->id;

            m_audioStreams.append(ai);
        }
    }

    avformat_close_input(&formatContext);

    emit print(tr("Found a total of %1 subtitle and %2 audio streams").arg(m_subStreams.count()).arg(m_audioStreams.count()), "FileInfo", MessageLevel::Info);

    if (m_subStreams.count() < 1)
    {
        emit print(tr("This file contains no subtitles!"), "FileInfo", MessageLevel::Warning);
    }

    if (m_audioStreams.count() < 1)
    {
        emit print(tr("This file contains no audio!"), "FileInfo", MessageLevel::Warning);
    }

    emit fileChanged(m_subStreams, m_audioStreams);

    return true;
}

} // namespace DialogueFromVideo
