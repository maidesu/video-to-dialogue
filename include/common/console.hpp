#pragma once

#include <common/singleton.hpp>

#include <QString>
#include <QVector>
#include <QList>
#include <QScrollBar>
#include <QTextEdit>

namespace DialogueFromVideo {

enum class MessageLevel
{
    Debug,
    Info,
    Warning,
    Error
};

struct Message
{
    QString msg;
    QString id;
    MessageLevel level;
};

class Console : public QObject, public Singleton<Console>
{
    Q_OBJECT

    friend class Singleton<Console>;

public:
    QTextEdit* textEdit() const { return m_textEdit; }

    const QStringList messageLevelLabels { "Debug",
                                           "Info",
                                           "Warning",
                                           "Error" };

    const QList<QColor> messageLevelColors { QColor(0xCCCCCC),
                                             QColor(0x45C6D6),
                                             QColor(0xFFC000),
                                             QColor(0xFF8080) };

public slots:
    void printHandler(const QString &msg,
                      const QString &id,
                      const DialogueFromVideo::MessageLevel level)
    {
#ifndef QT_DEBUG
        if (level == MessageLevel::Debug)
        {
            return;
        }
#endif

        if (level >= m_filterLevel)
        {
            printConsole(msg, id, level);
        }

        m_messageVec.push_back({ QString{msg},
                                 QString{id},
                                 MessageLevel{level} });
    }

    void filterHandler(const DialogueFromVideo::MessageLevel level)
    {
        clearConsole();

        m_filterLevel = MessageLevel{level};

        for (const Message& m : m_messageVec)
        {
            if (m.level >= m_filterLevel)
            {
                printConsole(m);
            }
        }
    }

    void clearHandler()
    {
        clearConsole();

        m_messageVec.clear();
    }

private:
    Console(QObject *parent = nullptr)
        : QObject(parent)
        , m_filterLevel(MessageLevel::Debug)
        , m_textEdit(new QTextEdit()) // Expects QWidget which we are not
    {
        m_textEdit->setReadOnly(true);
        m_textEdit->setStyleSheet("background-color: black;");
    }

    void printConsole(const Message& m) const
    {
        printConsole(m.msg, m.id, m.level);
    }

    void printConsole(const QString &msg,
               const QString &id,
               const DialogueFromVideo::MessageLevel level) const
    {
        m_textEdit->moveCursor(QTextCursor::End);

        m_textEdit->setTextColor(Qt::white);
        m_textEdit->insertPlainText("<");

        m_textEdit->setTextColor(messageLevelColors.at(static_cast<int>(level)));
        m_textEdit->insertPlainText(messageLevelLabels.at(static_cast<int>(level)));

        m_textEdit->setTextColor(QColor(0xDBA15A));
        m_textEdit->insertPlainText(QString(" %1").arg(id));

        m_textEdit->setTextColor(Qt::white);
        m_textEdit->insertPlainText(QString(">: %1\n").arg(msg));

        m_textEdit->ensureCursorVisible();
    }

    void clearConsole() const
    {
        m_textEdit->clear();
    }


    MessageLevel m_filterLevel;
    QTextEdit* m_textEdit;
    QVector<Message> m_messageVec;
};

} // namespace DialogueFromVideo
