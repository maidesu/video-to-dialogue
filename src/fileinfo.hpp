#pragma once

#include <QObject>
#include <QString>
#include <QFileDialog>
#include <QVector>

#include "common/messenger.hpp"

namespace DialogueFromVideo {

struct AudioInfo
{
    uint index;
    QString lang;
    QString codec;
    QString format;
    uint samplerate;
    uint bitdepth;
    uint bitrate;
    bool lossless;
};

struct SubInfo
{
    uint index;
    QString lang;
    QString format;
};

class FileInfo : public Messenger
{
    Q_OBJECT
public:
    explicit FileInfo(QObject *parent = nullptr);

    virtual ~FileInfo();

public slots:
    void openFileHandler() { openFile(); }

private:
    bool openFile();
    bool getFileInfoFfmpeg();

    const char* m_path;
    QList<SubInfo*> m_subStreams;
    QList<AudioInfo*> m_audioStreams;

signals:
    void fileChanged(const QList<DialogueFromVideo::SubInfo*>& subStreams,
                     const QList<DialogueFromVideo::AudioInfo*>& audioStreams);
};

} // namespace DialogueFromVideo
