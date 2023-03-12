#pragma once

#include <QString>
#include <QTextEdit>

#include "singleton.hpp"

namespace DialogueFromVideo {

enum class MessageLevel
{
    Debug,
    Info,
    Warning,
    Error
};

class Console : public QObject, public Singleton<Console>
{
    Q_OBJECT

public:
    Console(QObject *parent = nullptr) :
        QObject(parent),
        m_textEdit(new QTextEdit()) // Expects QWidget which we are not
    {
        m_textEdit->setReadOnly(true);
        m_textEdit->setStyleSheet("background-color: black; color: white;");
    }

    //~Console() { delete m_textEdit; }

    QTextEdit* textEdit() const { return m_textEdit; }

    const QStringList messageLevels { "Debug", "Info", "Warning", "Error" };

public slots:
    void printHandler(const QString &msg,
                      const QString &id,
                      const DialogueFromVideo::MessageLevel level) const
    {
        print(msg, id, level);
    }
    void clearHandler() const { clear(); }

private:
    void print(const QString &msg,
               const QString &id,
               const DialogueFromVideo::MessageLevel level) const
    {
        m_textEdit->append(QString("<%2, %3>:  %1").arg(msg,
                                                       id,
                                                       messageLevels.at((int)level)));
    }
    void clear() const { m_textEdit->clear(); }

    QTextEdit* m_textEdit;
};

} // namespace DialogueFromVideo
