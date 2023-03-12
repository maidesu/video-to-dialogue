#include "window.hpp"
#include "../common/console.hpp"

namespace DialogueFromVideo {

Window::Window(QWidget *parent) :
    QWidget(parent),
    m_layout(new QVBoxLayout(this))
{
    m_layout->addWidget(Console::instance().textEdit());
}

} // namespace DialogueFromVideo
