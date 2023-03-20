#include <QSplitter>

#include "window.hpp"
#include "../common/console.hpp"

namespace DialogueFromVideo {

Window::Window(QWidget *parent) :
    QWidget(parent),
    m_layout(new QVBoxLayout(this)),
    m_infoGroupBox(new QGroupBox("File details"))
{
    this->setMinimumSize(700, 400);


    QSplitter* splitter = new QSplitter();
    splitter->addWidget(new QWidget());
    splitter->addWidget(Console::instance().textEdit());
    splitter->setOrientation(Qt::Vertical);

    m_layout->addWidget(splitter);
    //this->setLayout(m_layout);
}

} // namespace DialogueFromVideo
