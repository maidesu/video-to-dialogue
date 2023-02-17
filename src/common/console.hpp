#pragma once

#include <QTextEdit>
#include <QVBoxLayout>

#include "singleton.hpp"

namespace DialogueFromVideo {

class Console : public QWidget, public Singleton<Console>
{
public:
    Console(QWidget *parent = nullptr) : QWidget(parent),
        m_layout(new QVBoxLayout(this)),
        m_textEdit(new QTextEdit(this))
    {
        m_textEdit->setReadOnly(true);
        m_textEdit->setTextColor(Qt::white);
        m_textEdit->setTextBackgroundColor(Qt::black);
        m_layout->addWidget(m_textEdit);
    }

    void print(const QString &msg) const { m_textEdit->append(msg); }
    void clear() const { m_textEdit->clear(); }

private:
    QVBoxLayout* m_layout;
    QTextEdit* m_textEdit;
};

} // namespace DialogueFromVideo
