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
                         &Console::printHandler); // Let's not allow Messengers to clear console
    }

    virtual ~Messenger() {}

signals:
    void print(const QString &msg,
               const QString &id,
               const DialogueFromVideo::MessageLevel &level = MessageLevel::Info);
};

}
