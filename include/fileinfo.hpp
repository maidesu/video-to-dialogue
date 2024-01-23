#pragma once

#include <file/open.hpp>
#include <common/messenger.hpp>

extern "C" {
#include <libavcodec/codec_id.h>
}

#include <QString>
#include <QVector>
#include <QFileDialog>
#include <QObject>

namespace DialogueFromVideo {

struct AudioInfo
{
    int index;
    int samplerate;
    int bitdepth;
    int64_t bitrate;
    bool lossless;
    AVCodecID codec_id;
    QString codec_name;
    QString lang;
};

struct SubInfo
{
    int index;
    AVCodecID codec_id;
    QString codec_name;
    QString lang;
};

class FileInfo : public Messenger
{
    Q_OBJECT
public:
    explicit FileInfo(QObject *parent = nullptr);

    FileInfo(FileInfo&) = delete;
    FileInfo(FileInfo&&) = delete;

    FileInfo& operator= (const FileInfo&) = delete;
    FileInfo& operator= (const FileInfo&&) = delete;

    virtual ~FileInfo();

public slots:
    void openFileHandler() { openFile(); }

    void subDescriptionRequestedHandler(const QString& index);

    void subLayerRequestedHandler(const QString& index);

    void audioDescriptionRequestedHandler(const QString& index);

private:
    bool openFile();
    bool getFileInfoFfmpeg();
    void clearStreamInfo();

    const char* m_path;
    int m_selectedSubIndex;
    int m_selectedSubLayerIndex;
    int m_selectedAudioIndex;

    QList<SubInfo*> m_subStreams;
    QList<AudioInfo*> m_audioStreams;

    File::Open* m_file;

signals:
    void fileChanged(const QList<DialogueFromVideo::SubInfo*>& subStreams,
                     const QList<DialogueFromVideo::AudioInfo*>& audioStreams);

    void subDescriptionReceivedSignal(const SubInfo subInfo);

    void audioDescriptionReceivedSignal(const AudioInfo audioInfo);
};

} // namespace DialogueFromVideo
