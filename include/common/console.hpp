#pragma once

#include "singleton.hpp"

#include <QString>
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
                      const DialogueFromVideo::MessageLevel level) const
    {
        print(msg, id, level);
    }
    void clearHandler() const { clear(); }

private:
    Console(QObject *parent = nullptr)
        : QObject(parent)
        , m_textEdit(new QTextEdit()) // Expects QWidget which we are not
    {
        m_textEdit->setReadOnly(true);
        m_textEdit->setStyleSheet("background-color: black;");
    }

    void print(const QString &msg,
               const QString &id,
               const DialogueFromVideo::MessageLevel level) const
    {
#ifndef QT_DEBUG
        if (level != MessageLevel::Debug) {
#endif
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
#ifndef QT_DEBUG
        }
#endif
    }
    void clear() const { m_textEdit->clear(); }

    QTextEdit* m_textEdit;
};

} // namespace DialogueFromVideo
