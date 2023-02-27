#pragma once

#include <QTextEdit>

#include "singleton.hpp"

namespace DialogueFromVideo {

class Console : public Singleton<Console>
{
public:
    Console() :
        m_textEdit(new QTextEdit())
    {
        m_textEdit->setReadOnly(true);
        m_textEdit->setTextColor(Qt::white);
        m_textEdit->setTextBackgroundColor(Qt::black);
    }

    ~Console() { delete m_textEdit; }

    QTextEdit* textEdit() const { return m_textEdit; }
    void print(const QString &msg) const { m_textEdit->append(msg); }
    void clear() const { m_textEdit->clear(); }

private:
    QTextEdit* m_textEdit;
};

} // namespace DialogueFromVideo
