#include <filemanager.hpp>

#include <file/write.hpp>
#include <file/remux.hpp>

#include <cstring>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
}

namespace DialogueFromVideo {

FileManager::FileManager(QObject* parent)
    : QObject(parent)
    , m_path(nullptr)
    , m_selectedVideoIndex(-1)
    , m_selectedAudioIndex(-1)
    , m_selectedSubIndex(-1)
    , m_selectedSubLayerIndex(-1)
    , m_file(nullptr)
{

}

FileManager::~FileManager()
{
    if (m_file != nullptr)
    {
        delete m_file;
        m_file = nullptr;
    }

    for (SubInfo* si : m_subStreams)
    {
        delete si;
    }
    for (AudioInfo* ai : m_audioStreams)
    {
        delete ai;
    }
}

void FileManager::openFileHandler()
{
    openFile();
}

void FileManager::exportSubtitleHandler(const QTextEdit *textEdit)
{
    saveFile(SaveMode::Plaintext,
             FileMode::None,
             textEdit);
}

void FileManager::exportPictureCollectionHandler(const QTextEdit *textEdit)
{
    saveFile(SaveMode::Slides,
             FileMode::None,
             textEdit);
}

void FileManager::exportDialogueHandler()
{
    saveFile(SaveMode::Extract);
}

void FileManager::exportVideoRemuxHandler()
{
    saveFile(SaveMode::Remux,
             FileMode::Video);
}

void FileManager::exportAudioRemuxHandler()
{
    saveFile(SaveMode::Remux,
             FileMode::Audio);
}

void FileManager::exportSubtitleRemuxHandler()
{
    saveFile(SaveMode::Remux,
             FileMode::Subtitle);
}

void FileManager::subDescriptionRequestedHandler(const QString& index)
{
    // Let the default state be -1, since index should be >=0
    m_selectedSubIndex = -1;

    if (index.isEmpty())
    {
        return;
    }

    emit m_messenger.print(QString("subDescriptionRequestedHandler(%1)").arg(index),
                           "FileManager",
                           MessageLevel::Debug);

    int idx = index.toInt();

    if (idx < 0)
    {
        emit m_messenger.print(tr("No subtitle stream selected"),
                               "FileManager",
                               MessageLevel::Info);
        return;
    }

    for (const SubInfo* si : m_subStreams)
    {
        if (si->index == idx)
        {
            m_selectedSubIndex = idx;

            emit m_messenger.print(QString("Selected subtitle stream %1").arg(QString::number(m_selectedSubIndex)),
                                   "FileManager",
                                   MessageLevel::Debug);

            emit subDescriptionReceivedSignal(SubInfo(*si)); // Call to default copy ctor

            return;
        }
    }
}

void FileManager::subLayerRequestedHandler(const QString& index)
{
    // Let the default state be -1, since index should be >=0
    m_selectedSubLayerIndex = -1;

    if (index.isEmpty())
    {
        return;
    }

    emit m_messenger.print(QString("subLayerRequestedHandler(%1)").arg(index),
                           "FileManager",
                           MessageLevel::Debug);

    int idx = index.toInt();

    if (idx < 0)
    {
        emit m_messenger.print(tr("No subtitle layer selected"),
                               "FileManager",
                               MessageLevel::Info);
        return;
    }

    // TODO: A parser can perform checks

    m_selectedSubLayerIndex = idx;

    emit m_messenger.print(QString("Selected subtitle layer %1").arg(QString::number(m_selectedSubLayerIndex)),
                           "FileManager",
                           MessageLevel::Debug);
}

void FileManager::audioDescriptionRequestedHandler(const QString& index)
{
    // Let the default state be -1, since index should be >=0
    m_selectedAudioIndex = -1;

    if (index.isEmpty())
    {
        return;
    }

    emit m_messenger.print(QString("audioDescriptionRequestedHandler(%1)").arg(index),
                           "FileManager",
                           MessageLevel::Debug);

    int idx = index.toInt();

    if (idx < 0)
    {
        emit m_messenger.print(tr("No audio stream selected"),
                               "FileManager",
                               MessageLevel::Info);
        return;
    }

    for (const AudioInfo* ai : m_audioStreams)
    {
        if (ai->index == idx)
        {
            m_selectedAudioIndex = idx;

            emit m_messenger.print(QString("Selected audio stream %1").arg(QString::number(m_selectedAudioIndex)),
                                   "FileManager",
                                   MessageLevel::Debug);

            emit audioDescriptionReceivedSignal(AudioInfo(*ai)); // Call to default copy ctor

            return;
        }
    }
}

void FileManager::processFileHandler()
{
    if (-1 != m_selectedSubIndex)
    {
        emit subtitleRequestedSignal(m_file,
                                     m_selectedSubIndex,
                                     m_selectedSubLayerIndex);
    }

    if (-1 != m_selectedAudioIndex)
    {
        emit waveFormRequestedSignal(m_file,
                                     m_selectedAudioIndex);
    }

    emit m_messenger.print(tr("Processed file"),
                           "FileManager",
                           MessageLevel::Info);
}

bool FileManager::openFile()
{
    QString openPath =
            QFileDialog::getOpenFileName(nullptr,
                                         tr("Open File"),
                                         "",
                                         tr("Video ("
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
                                            ");;All formats (*)"));

    if (!openPath.isEmpty())
    {
        QByteArray bytes = openPath.toUtf8();
        m_path = bytes.constData();

        emit m_messenger.print(tr("Selected file: %1").arg(openPath),
                               "FileManager",
                               MessageLevel::Info);

        emit m_messenger.print(QString("Selected path: %1").arg(m_path),
                               "FileManager",
                               MessageLevel::Debug);

        return getFileInfo(); // note: m_path is invalid after this point
    }
    else
    {
        emit m_messenger.print(tr("Received no file path!"),
                               "FileManager",
                               MessageLevel::Warning);

        return false;
    }
}

bool FileManager::saveFile(SaveMode saveMode,
                           FileMode fileMode,
                           const QTextEdit* textEdit)
{
    QString savePath;

    switch (saveMode)
    {
        default:
        case SaveMode::None:
            return false;
            break;

        case SaveMode::Plaintext:
            savePath = QFileDialog::getSaveFileName(nullptr,
                                                    tr("Export Subtitle"),
                                                    "",
                                                    tr("Text ("
                                                       "*.txt"
                                                       ");;All formats (*)"));

            if (savePath.isEmpty())
            {
                emit m_messenger.print(tr("Received no file path!"),
                                       "FileManager",
                                       MessageLevel::Warning);

                return false;
            }

            {
                QFile file_out(savePath);

                if (!file_out.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
                {
                    emit m_messenger.print(tr("Failed to save at specified location!"),
                                           "FileManager",
                                           MessageLevel::Error);

                    return false;
                }

                QTextStream stream_out(&file_out);
                stream_out << textEdit->toPlainText();

                file_out.close();
            }

            emit m_messenger.print(tr("Saved subtitles to file: %1").arg(savePath),
                                   "FileManager",
                                   MessageLevel::Info);

            break;

        case SaveMode::Slides:
            savePath = QFileDialog::getSaveFileName(nullptr,
                                                    tr("Export Picture Collection"));
            break;

        case SaveMode::Extract:
            throw std::logic_error("Not implemented"); // TODO
            break;

        case SaveMode::Remux:
            int selectedStream;
            char formattedExtensions[256] = {0};
            char* format_write = formattedExtensions;

            switch (fileMode)
            {
                default:
                case FileMode::None:
                    return false;

                case FileMode::Video: // Containers may support many videos, will remux the first
                    selectedStream = m_selectedVideoIndex;
                    strcpy(formattedExtensions, "Video");
                    break;

                case FileMode::Audio:
                    selectedStream = m_selectedAudioIndex;
                    strcpy(formattedExtensions, "Audio");
                    break;

                case FileMode::Subtitle:
                    selectedStream = m_selectedSubIndex;
                    strcpy(formattedExtensions, "Subtitle");
                    break;
            }

            const char* short_name = avcodec_get_name(m_file->getStream(selectedStream)->codecpar->codec_id);

            const AVOutputFormat* outFormat = av_guess_format(short_name,
                                                              nullptr,
                                                              nullptr);


            while (*format_write++ != '\0');
            --format_write; // Move to right after the text

            *format_write++ = ' ';
            *format_write++ = '(';

            if (outFormat)
            {
                *format_write++ = '*';
                *format_write++ = '.';

                const char* exts = outFormat->extensions;

                for (;;)
                {
                    if (*exts == '\0')
                    {
                        break;
                    }

                    if (*exts == ',')
                    {
                        *format_write++ = ' ';
                        *format_write++ = '*';
                        *format_write++ = '.';
                    }
                    else
                    {
                        *format_write++ = *exts;
                    }

                    ++exts;
                }

                emit m_messenger.print(QString("Found extensions: %1").arg(outFormat->extensions),
                                       "FileManager",
                                       MessageLevel::Debug);
            }
            else
            {
                *format_write++ = '*';

                emit m_messenger.print(tr("Did not find corresponding extensions!"),
                                       "FileManager",
                                       MessageLevel::Warning);
            }

            *format_write++ = ')';
            *format_write++ = ';';
            *format_write++ = ';';
            *format_write   = '\0';

            savePath = QFileDialog::getSaveFileName(nullptr,
                                                    tr("Export File"),
                                                    "",
                                                    tr("%1All formats (*)").arg(formattedExtensions));

            if (savePath.isEmpty())
            {
                emit m_messenger.print(tr("Received no file path!"),
                                       "FileManager",
                                       MessageLevel::Warning);

                return false;
            }

            File::Write write(savePath.toUtf8().constData());

            if (write.getResult() < 0)
            {
                return false;
            }

            File::Remux remux(m_file->getContext(),
                              write.getContext(),
                              selectedStream);

            if (remux.getResult() < 0)
            {
                return false;
            }

            break;
    }

    return true;
}

bool FileManager::getFileInfo()
{
    FileManager::clearStreamInfo();

    emit m_progress.progressReset();

    // Validate File class
    if (m_file != nullptr)
    {
        delete m_file;
        m_file = nullptr;
    }

    m_file = new File::Read(m_path);

    if (m_file->getResult() < 0)
    {
        delete m_file;
        m_file = nullptr;
        return false;
    }

    // Get file info
    AVStream* stream;
    for (uint i = 0; i < m_file->getStreamCount(); ++i)
    {
        stream = m_file->getStream(i);

        switch (stream->codecpar->codec_type)
        {
            default:
            case AVMEDIA_TYPE_UNKNOWN:
            case AVMEDIA_TYPE_DATA:
            case AVMEDIA_TYPE_ATTACHMENT:
            case AVMEDIA_TYPE_NB:
                break;

            case AVMEDIA_TYPE_VIDEO:
                m_selectedVideoIndex = i;
                break;

            case AVMEDIA_TYPE_AUDIO:
            {
                AudioInfo* ai = new AudioInfo();

                const AVCodecDescriptor* desc = avcodec_descriptor_get(stream->codecpar->codec_id);

                ai->index       = stream->index;

                ai->samplerate  = stream->codecpar->sample_rate;

                ai->bitdepth    = stream->codecpar->bits_per_coded_sample == 0
                                   ? stream->codecpar->bits_per_raw_sample == 0 : stream->codecpar->bits_per_coded_sample
                                         ? av_get_bits_per_sample(stream->codecpar->codec_id) : stream->codecpar->bits_per_raw_sample;

                ai->bitrate     = stream->codecpar->bit_rate / 1000;

                ai->lossless    = desc->props ? static_cast<bool>(desc->props & AV_CODEC_PROP_LOSSLESS) : false;

                ai->codec_id    = stream->codecpar->codec_id;

                ai->codec_name  = QString(avcodec_get_name(ai->codec_id)
                                             ? avcodec_get_name(ai->codec_id)
                                             : "N/A" );

                ai->lang        = QString(av_dict_get(stream->metadata, "language", nullptr, 0)
                                       ? av_dict_get(stream->metadata, "language", nullptr, 0)->value
                                       : "N/A");

                m_audioStreams.append(ai);

                emit m_messenger.print("Appended audio stream",
                                       "FileManager",
                                       MessageLevel::Debug);
            }

                break;

            case AVMEDIA_TYPE_SUBTITLE:
            {
                SubInfo* si = new SubInfo();

                si->index       = stream->index;

                si->codec_id    = stream->codecpar->codec_id;

                si->codec_name  = QString(avcodec_get_name(si->codec_id)
                                             ? avcodec_get_name(si->codec_id)
                                             : "N/A" );

                si->lang        = QString(av_dict_get(stream->metadata, "language", nullptr, 0)
                                       ? av_dict_get(stream->metadata, "language", nullptr, 0)->value
                                       : "N/A");

                m_subStreams.append(si);

                emit m_messenger.print("Appended sub stream",
                                       "FileManager",
                                       MessageLevel::Debug);
            }

                break;
        }
    }

    emit m_messenger.print(tr("Found a total of %1 subtitle and %2 audio streams")
                                .arg(QString::number(m_subStreams.count()),
                                QString::number(m_audioStreams.count())),
                           "FileManager",
                           MessageLevel::Info);

    if (m_subStreams.count() < 1)
    {
        emit m_messenger.print(tr("This file contains no subtitles!"),
                               "FileManager",
                               MessageLevel::Warning);
    }

    if (m_audioStreams.count() < 1)
    {
        emit m_messenger.print(tr("This file contains no audio!"),
                               "FileManager",
                               MessageLevel::Warning);
    }

    emit fileChangedSignal(m_selectedVideoIndex == -1 ? 0 : 1,
                           m_audioStreams,
                           m_subStreams);

    return true;
}

void FileManager::clearStreamInfo()
{
    m_selectedVideoIndex = -1;
    m_selectedAudioIndex = -1;
    m_selectedSubIndex = -1;
    m_selectedSubLayerIndex = -1;

    while (!m_subStreams.isEmpty())
    {
        delete m_subStreams.takeLast();
    }
    while (!m_audioStreams.isEmpty())
    {
        delete m_audioStreams.takeLast();
    }
}

} // namespace DialogueFromVideo
