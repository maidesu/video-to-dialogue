#pragma once

#include <QObject>
#include <QString>
#include <QFileDialog>
#include <QVector>

#include "common/messenger.hpp"

namespace DialogueFromVideo {

class FileInfo : public Messenger
{
    Q_OBJECT
public:
    FileInfo(QObject *parent = nullptr) :
        Messenger(parent) {}

    void openFile();

private:
    void getFileInfoFfmpeg();

    const char* path;
    QStringList subStreams;
    QStringList audioStreams;

signals:
    void fileChanged(const QStringList &subStreams, const QStringList &audioStreams);
};

} // namespace DialogueFromVideo
