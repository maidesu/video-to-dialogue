#include "../include/fileinfo.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

namespace DialogueFromVideo {

FileInfo::FileInfo(QObject *parent)
    : Messenger(parent)
    , m_path(nullptr)
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

void FileInfo::subDescriptionRequestedHandler(const QString& index)
{
    int idx = index.toInt();

    if (idx < 1)
    {
        return;
    }

    emit print(QString("subDescriptionRequestedHandler(%1)").arg(index),
               "FileInfo",
               MessageLevel::Debug);

    for (const SubInfo* si : m_subStreams)
    {
        if (si->index == idx)
        {
            emit subDescriptionReceivedSignal(SubInfo(*si)); // Call to default copy ctor
            return;
        }
    }
}

void FileInfo::audioDescriptionRequestedHandler(const QString& index)
{
    int idx = index.toInt();

    if (idx < 1)
    {
        return;
    }

    emit print(QString("audioDescriptionRequestedHandler(%1)").arg(index),
               "FileInfo",
               MessageLevel::Debug);

    for (const AudioInfo* ai : m_audioStreams)
    {
        if (ai->index == idx)
        {
            emit audioDescriptionReceivedSignal(AudioInfo(*ai)); // Call to default copy ctor
            return;
        }
    }
}

bool FileInfo::openFile()
{
    QString openPath =
            QFileDialog::getOpenFileName(nullptr,
                                         tr("Open File"),
                                         "",
                                         tr("Video Files ("
                                            "*.mp4 "
                                            "*.m4a "
                                            "*.m4v "
                                            "*.mov "
                                            "*.avi "
                                            "*.mkv "
                                            "*.flv "
                                            "*.wmv "
                                            "*.qt "
                                            "*.ts "
                                            "*.mpg "
                                            "*.mpeg "
                                            "*.ogv "
                                            "*.m2ts"
                                            ");;All Files (*)"));

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

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        emit print(tr("Failed to fill in missing stream information!"), "FileInfo", MessageLevel::Warning);
    }

    // At this point we drop all content from before
    m_subStreams.clear();
    m_audioStreams.clear();

    AVStream* stream = nullptr;
    for (uint i = 0; i < formatContext->nb_streams; ++i) {
        stream = formatContext->streams[i];

        if (stream->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE) {
            SubInfo* si = new SubInfo();

            si->index       = stream->index;

            si->lang        = QString(av_dict_get(stream->metadata, "language", nullptr, 0)
                                   ? av_dict_get(stream->metadata, "language", nullptr, 0)->value : "N/A");

            si->format      = QString(formatContext->oformat ? formatContext->oformat->name : "N/A");

            m_subStreams.append(si);
        }
        else if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            AudioInfo* ai = new AudioInfo();

            const AVCodecDescriptor* desc = avcodec_descriptor_get(stream->codecpar->codec_id);

            ai->index       = stream->index;

            ai->samplerate  = stream->codecpar->sample_rate;

            ai->bitdepth    = stream->codecpar->bits_per_coded_sample == 0
                                ? stream->codecpar->bits_per_raw_sample == 0 : stream->codecpar->bits_per_coded_sample
                                    ? av_get_bits_per_sample(stream->codecpar->codec_id) : stream->codecpar->bits_per_raw_sample;

            ai->bitrate     = stream->codecpar->bit_rate / 1000;

            ai->lossless    = desc->props ? static_cast<bool>(desc->props & AV_CODEC_PROP_LOSSLESS) : false;

            ai->lang        = QString(av_dict_get(stream->metadata, "language", nullptr, 0)
                                        ? av_dict_get(stream->metadata, "language", nullptr, 0)->value : "N/A");

            ai->codec       = QString(avcodec_find_decoder(stream->codecpar->codec_id)
                                        ? avcodec_find_decoder(stream->codecpar->codec_id)->name : "N/A");

            ai->format      = QString(formatContext->oformat ? formatContext->oformat->name : "N/A");

            m_audioStreams.append(ai);
        }
    }

    avformat_close_input(&formatContext);

    emit print(tr("Found a total of %1 subtitle and %2 audio streams")
                   .arg(QString::number(m_subStreams.count()),
                        QString::number(m_audioStreams.count())),
               "FileInfo",
               MessageLevel::Info);

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
