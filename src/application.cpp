#include "application.hpp"

namespace DialogueFromVideo {

Application::Application() :
    m_console(Console::instance())
{
}

void Application::run()
{
    m_window.show();

    m_console.print("First console entry");
    m_console.print("Second console entry!");
}

} // namespace DialogueFromVideo
