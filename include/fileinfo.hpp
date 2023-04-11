#pragma once

#include "common/messenger.hpp"

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
    QString lang;
    QString codec;
    QString format;
};

struct SubInfo
{
    int index;
    QString lang;
    QString format;
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

    void audioDescriptionRequestedHandler(const QString& index);

private:
    bool openFile();
    bool getFileInfoFfmpeg();

    const char* m_path;
    QList<SubInfo*> m_subStreams;
    QList<AudioInfo*> m_audioStreams;

signals:
    void fileChanged(const QList<DialogueFromVideo::SubInfo*>& subStreams,
                     const QList<DialogueFromVideo::AudioInfo*>& audioStreams);

    void subDescriptionReceivedSignal(const SubInfo subInfo);

    void audioDescriptionReceivedSignal(const AudioInfo audioInfo);
};

} // namespace DialogueFromVideo
