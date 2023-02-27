#pragma once

#include <QObject>
#include <QString>
#include <QFileDialog>
#include <QVector>

namespace DialogueFromVideo {

class File : public QObject
{
    Q_OBJECT
public:
    File(QObject *parent = nullptr) :
        QObject(parent) {}

    QString openFilePath();
    void getAudioCodecs();

private:
    QString path;
    QVector<QString> audioCodecs;
};

} // namespace DialogueFromVideo
