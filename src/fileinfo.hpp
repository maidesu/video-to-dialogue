#pragma once

#include <QObject>
#include <QString>
#include <QFileDialog>
#include <QVector>

namespace DialogueFromVideo {

class FileInfo : public QObject
{
    Q_OBJECT
public:
    FileInfo(QObject *parent = nullptr) :
        QObject(parent) {}

    QString openFilePath();
    void getAudioCodecs();

private:
    QString path;
    QStringList audioCodecs;
};

} // namespace DialogueFromVideo
