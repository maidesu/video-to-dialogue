#pragma once

#include <QObject>
#include "console.hpp"

namespace DialogueFromVideo {

class Messenger : public QObject
{
    Q_OBJECT
public:
    Messenger(QObject *parent = nullptr) :
        QObject(parent)
    {
        QObject::connect(this,
                         &Messenger::print,
                         &Console::instance(),
                         &Console::printHandler);
    }

signals:
    void print(const QString &msg,
               const QString &id,
               const DialogueFromVideo::MessageLevel &level = MessageLevel::Info);
};

}
