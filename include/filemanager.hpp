#pragma once

#include <file/read.hpp>
#include <common/messenger.hpp>
#include <common/progress.hpp>

extern "C"
{
    #include <libavcodec/codec_id.h>
}

#include <QString>
#include <QList>
#include <QFileDialog>
#include <QObject>

namespace DialogueFromVideo {

enum class SaveMode
{
    None,
    Plaintext,
    Slides,
    Extract,
    Remux
};

enum class FileMode
{
    None,
    Video,
    Audio,
    Subtitle
};

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

class FileManager : public QObject
{
    Q_OBJECT
public:
    explicit FileManager(QObject *parent = nullptr);

    FileManager(FileManager&) = delete;
    FileManager(FileManager&&) = delete;

    FileManager& operator= (const FileManager&) = delete;
    FileManager& operator= (const FileManager&&) = delete;

    virtual ~FileManager();

public slots:
    void openFileHandler();

    void exportSubtitleHandler(const QTextEdit* textEdit);

    void exportPictureCollectionHandler(const QTextEdit* textEdit);

    void exportDialogueHandler();

    void exportVideoRemuxHandler();

    void exportAudioRemuxHandler();

    void exportSubtitleRemuxHandler();

    void subDescriptionRequestedHandler(const QString& index);

    void subLayerRequestedHandler(const QString& index);

    void audioDescriptionRequestedHandler(const QString& index);

private:
    bool openFile();
    bool saveFile(SaveMode saveMode = SaveMode::None,
                  FileMode fileMode = FileMode::None,
                  const QTextEdit* textEdit = nullptr);
    bool getFileInfo();
    void clearStreamInfo();

    const char* m_path;
    int m_selectedVideoIndex;
    int m_selectedAudioIndex;
    int m_selectedSubIndex;
    int m_selectedSubLayerIndex;

    QList<SubInfo*> m_subStreams;
    QList<AudioInfo*> m_audioStreams;

    File::Read* m_file;

    Messenger m_messenger;
    Progress m_progress;

signals:
    void fileChangedSignal(int videoStream,
                           const QList<DialogueFromVideo::AudioInfo*>& audioStreams,
                           const QList<DialogueFromVideo::SubInfo*>& subStreams);

    void subDescriptionReceivedSignal(const SubInfo subInfo);

    void audioDescriptionReceivedSignal(const AudioInfo audioInfo);

    void subtitleRequestedSignal(File::Read* file,
                                 int selectedSubIndex,
                                 int selectedSubLayerIndex);

    void waveFormRequestedSignal(File::Read* file,
                                 int selectedAudioIndex);
};

} // namespace DialogueFromVideo
