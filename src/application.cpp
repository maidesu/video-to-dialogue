#include "application.hpp"

namespace DialogueFromVideo {

Application::Application() :
    m_console(Console::instance())
{
}

void Application::run()
{
    m_window.show();

    emit m_fileinfo.print("First console entry from FileInfo", "FileInfo", MessageLevel::Debug);
    emit m_fileinfo.print("Second console entry!", "FileInfo");
}

} // namespace DialogueFromVideo
