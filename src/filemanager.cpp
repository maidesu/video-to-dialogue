#include <filemanager.hpp>

#include <file/write.hpp>
#include <file/remux.hpp>
#include <file/transcode.hpp>
#include <common/time.hpp>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
}

#include <QPainter>

#include <cstring>

namespace DialogueFromVideo {

FileManager::FileManager(QObject* parent)
    : QObject(parent)
    , m_path(nullptr)
    , m_imageIndex(0)
    , m_selectedVideoIndex(-1)
    , m_selectedAudioIndex(-1)
    , m_selectedSubIndex(-1)
    , m_selectedSubLayerIndex(-1)
    , m_dialogueFlag(0)
    , m_file(nullptr)
    , m_dialogueList(nullptr)
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

void FileManager::exportDialogueHandler(FormatOptions::Option option)
{
    saveFile(SaveMode::Extract,
             FileMode::None,
             nullptr,
             option);
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

    // A parser can perform checks

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

void FileManager::sampleRateRequestedHandler(int& sampleRate)
{
    const AVStream* stream = m_file->getStream(m_selectedAudioIndex);

    if (stream->codecpar->sample_rate > 1)
    {
        sampleRate = stream->codecpar->sample_rate;
    }
    else
    {
        sampleRate = 48000;
    }
}

void FileManager::processFileHandler()
{
    if (-1 != m_selectedAudioIndex)
    {
        emit waveFormRequestedSignal(m_file,
                                     m_selectedAudioIndex);
    }

    if (-1 != m_selectedSubIndex)
    {
        emit subtitleRequestedSignal(m_file,
                                     m_selectedSubIndex,
                                     m_selectedSubLayerIndex);
    }


    emit m_messenger.print(tr("Processed file"),
                           "FileManager",
                           MessageLevel::Info);


    if ( m_audioStreams.count() > 0 &&
         m_dialogueFlag == 1 )
    {
        emit allowDialogueExportSignal();

        emit m_messenger.print(tr("File ready for dialogue export!"),
                               "FileManager",
                               MessageLevel::Info);
    }
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

        // Invalidate dialogue export
        m_dialogueList = nullptr;
        m_dialogueFlag = 0;

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
                           const QTextEdit* textEdit,
                           FormatOptions::Option option)
{
    switch (saveMode)
    {
        default:
        case SaveMode::None:
            return false;
            break;

        case SaveMode::Plaintext:
            m_savePath = QFileDialog::getSaveFileName(nullptr,
                                                      tr("Export Subtitle"),
                                                      "",
                                                      tr("Text ("
                                                         "*.txt"
                                                         ");;All formats (*)"));

            if (m_savePath.isEmpty())
            {
                emit m_messenger.print(tr("Received no file path!"),
                                       "FileManager",
                                       MessageLevel::Warning);

                return false;
            }

            {
                QFile file_out(m_savePath);

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

            emit m_messenger.print(tr("Saved subtitles to file: %1").arg(m_savePath),
                                   "FileManager",
                                   MessageLevel::Info);

            break;

        case SaveMode::Slides:
            m_savePath = QFileDialog::getExistingDirectory(nullptr,
                                                           tr("Export Visual Novel"));

            if (m_savePath.isEmpty())
            {
                emit m_messenger.print(tr("Received no file path!"),
                                       "FileManager",
                                       MessageLevel::Warning);

                return false;
            }

            {
                QString text = textEdit->toPlainText();
                QStringList lines = text.split("\n");

                /* Check window subtitle content is at least 2 lines
                 * and after that assume that the format is correct */
                if (lines.size() < 2)
                {
                    emit m_messenger.print(tr("There is nothing to make a visual novel out of!"),
                                           "FileManager",
                                           MessageLevel::Error);

                    return false;
                }

                m_imageIndex = 0;

                // Progress
                emit m_progress.progressReset();
                emit m_progress.progressMaximum(lines.size() / 2);

                for (int i = 0; i + 1 < lines.size(); i += 2)
                {
                    emit frameRequestedSignal(m_file,
                                              Time::stringTimeToMilliseconds(lines[i].left(12)),
                                              lines[i+1],
                                              m_selectedVideoIndex);
                }

                emit m_messenger.print(tr("Created visual novel of %1 images in: %2")
                                           .arg(QString::number(m_imageIndex),
                                                m_savePath),
                                       "FileManager",
                                       MessageLevel::Info);

                // Progress
                emit m_progress.progressComplete();
            }

            break;

        case SaveMode::Extract:
            m_savePath = QFileDialog::getSaveFileName(nullptr,
                                                      tr("Export Dialogue"),
                                                      "",
                                                      tr("All formats (*)"));

            if (m_savePath.isEmpty())
            {
                emit m_messenger.print(tr("Received no file path!"),
                                       "FileManager",
                                       MessageLevel::Warning);

                return false;
            }

            {
                File::Write write(m_savePath.toUtf8().constData(),
                                  av_guess_format(avcodec_get_name(static_cast<AVCodecID>(option.id)),
                                                  m_savePath.toUtf8().constData(),
                                                  NULL));

                if (write.getResult() < 0)
                {
                    return false;
                }

                File::Transcode transcode(m_file->getContext(),
                                          write.getContext(),
                                          static_cast<AVCodecID>(option.id),
                                          m_dialogueList,
                                          m_selectedAudioIndex);

                if (transcode.getResult() < 0)
                {
                    return false;
                }

                emit m_messenger.print(tr("Finished exporting dialogue"),
                                       "FileManager",
                                       MessageLevel::Info);
            }

            break;

        case SaveMode::Remux:
            int selectedStream;
            char exts[256] = {0};
            char prefix[16] = {0};

            switch (fileMode)
            {
                default:
                case FileMode::None:
                    return false;

                case FileMode::Video: // Containers may support many videos, will remux the first
                    selectedStream = m_selectedVideoIndex;
                    strcpy(prefix, "Video");
                    break;

                case FileMode::Audio:
                    selectedStream = m_selectedAudioIndex;
                    strcpy(prefix, "Audio");
                    break;

                case FileMode::Subtitle:
                    selectedStream = m_selectedSubIndex;
                    strcpy(prefix, "Subtitle");
                    break;
            }

            const char* short_name = avcodec_get_name(m_file->getStream(selectedStream)->codecpar->codec_id);

            const AVOutputFormat* outFormat = av_guess_format(short_name,
                                                              m_file->getContext()->url,
                                                              nullptr);

            emit m_messenger.print(short_name, "FileManager", MessageLevel::Debug);

            getFormatExtensions(exts, prefix, outFormat);

            m_savePath = QFileDialog::getSaveFileName(nullptr,
                                                      tr("Export File"),
                                                      "",
                                                      tr("%1All formats (*)").arg(exts));

            if (m_savePath.isEmpty())
            {
                emit m_messenger.print(tr("Received no file path!"),
                                       "FileManager",
                                       MessageLevel::Warning);

                return false;
            }

            {
                File::Write write(m_savePath.toUtf8().constData(), outFormat);

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
            }

            break;
    }

    return true;
}

void FileManager::frameReadyHandler(const QVector<uint8_t>& frameBinaryData,
                                    int width,
                                    int height,
                                    QString caption)
{
    m_imageIndex += 1;

    QString path = m_savePath + "/" + QString::number(m_imageIndex) + ".png";
    QImage image(frameBinaryData.constData(),
                 width,
                 height,
                 QImage::Format_RGB888);

    //image.loadFromData(frameBinaryData);

    // Insert gradient on top
    QImage scaledGradient = QImage(":/gradient.png")
                                .scaled(image.size(),
                                        Qt::IgnoreAspectRatio,
                                        Qt::SmoothTransformation);

    QPixmap merged(image.width(),
                   image.height());

    //merged.fill(Qt::transparent);

    QPainter painter(&merged);
    painter.drawImage(QRect(0, 0, image.width(), image.height()),
                      image);
    painter.drawImage(QRect(0, 0, image.width(), image.height()),
                      scaledGradient);

    // Insert caption on top
    painter.setPen(QPen(QColor(255, 255, 255)));

    painter.setFont(QFont("Helvetica",
                          image.width() > 1920 ? 48 : 24,
                          QFont::DemiBold));

    QRect textRect(QPoint(17 * image.width() / 200, // approx: 325 / 3840
                          17 * image.height() / 24),  // 1485 / 2160 - modified this
                   QPoint((200-17) * image.width() / 200, // Same spacing from the right
                          23 * image.height() / 24));

    painter.drawText(textRect,
                     Qt::AlignLeft | Qt::TextWordWrap,
                     caption);

    // Save the QImage as a PNG file
    if (!merged.toImage().save(path, "PNG"))
    {
        emit m_messenger.print(tr("Failed to save frame image at specified location!"),
                               "FileManager",
                               MessageLevel::Error);
    }

    // Progress
    emit m_progress.progressAdd(1);
}

void FileManager::readyDialogueHandler(const QList<Interval>& dialogue)
{
    m_dialogueList = &dialogue;

    // Current state of dialogue is ready
    // Invalidated on file change
    m_dialogueFlag = 1;
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

                ai->bitdepth    = stream->codecpar->bits_per_raw_sample
                                    ? stream->codecpar->bits_per_raw_sample
                                    : stream->codecpar->bits_per_coded_sample;

                ai->bitrate     = stream->codecpar->bit_rate / 1000;

                ai->channels    = stream->codecpar->ch_layout.nb_channels;

                ai->lossless    = static_cast<bool>(desc->props & AV_CODEC_PROP_LOSSLESS);

                ai->codec_id    = stream->codecpar->codec_id;

                ai->codec_name  = QString(avcodec_get_name(ai->codec_id));

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

                si->codec_name  = QString(avcodec_get_name(si->codec_id));

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

void FileManager::getFormatExtensions(char* dst,
                                      const char* prefix,
                                      const AVOutputFormat* of)
{
    strcpy(dst, prefix);

    while (*dst++ != '\0');
    --dst; // Move to right after the text

    *dst++ = ' ';
    *dst++ = '(';

    if (dst)
    {
        *dst++ = '*';
        *dst++ = '.';

        const char* of_exts = of->extensions;

        for (;;)
        {
            if (*of_exts == '\0')
            {
                break;
            }

            if (*of_exts == ',')
            {
                *dst++ = ' ';
                *dst++ = '*';
                *dst++ = '.';
            }
            else
            {
                *dst++ = *of_exts;
            }

            ++of_exts;
        }

        emit m_messenger.print(QString("Found extensions: %1").arg(of->extensions),
                               "FileManager",
                               MessageLevel::Debug);
    }
    else
    {
        *dst++ = '*';

        emit m_messenger.print(tr("Did not find corresponding extensions!"),
                               "FileManager",
                               MessageLevel::Warning);
    }

    *dst++ = ')';
    *dst++ = ';';
    *dst++ = ';';
    *dst   = '\0';
}

} // namespace DialogueFromVideo
