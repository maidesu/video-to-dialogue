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

class FileInfo : public QObject
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

    void exportSubtitleHandler(const QTextEdit* textEdit) { saveFile(SaveMode::Plaintext,
                                                                     FileMode::None,
                                                                     textEdit); }

    void exportPictureCollectionHandler(const QTextEdit* textEdit) { saveFile(SaveMode::Slides,
                                                                              FileMode::None,
                                                                              textEdit); }

    void exportDialogueHandler() { saveFile(SaveMode::Extract); }

    void exportVideoRemuxHandler() { saveFile(SaveMode::Remux,
                                              FileMode::Video); }

    void exportAudioRemuxHandler() { saveFile(SaveMode::Remux,
                                              FileMode::Audio); }

    void exportSubtitleRemuxHandler() { saveFile(SaveMode::Remux,
                                                 FileMode::Subtitle); }

    void subDescriptionRequestedHandler(const QString& index);

    void subLayerRequestedHandler(const QString& index);

    void audioDescriptionRequestedHandler(const QString& index);

private:
    bool openFile();
    bool saveFile(SaveMode saveMode = SaveMode::None,
                  FileMode fileMode = FileMode::None,
                  const QTextEdit* textEdit = nullptr);
    bool getFileInfoFfmpeg();
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
};

} // namespace DialogueFromVideo
