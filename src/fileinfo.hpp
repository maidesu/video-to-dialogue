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

    QString openFilePath();
    void getAudioCodecs();

private:
    QString path;
    QStringList audioCodecs;
};

} // namespace DialogueFromVideo
