#include <QTranslator>
#include <QCoreApplication>

#include "application.hpp"

namespace DialogueFromVideo {

Application::Application() :
    m_console(Console::instance())
{
    QObject::connect(&m_window,
                     &Window::openFileSignal,
                     &m_fileinfo,
                     &FileInfo::openFileHandler);

    QObject::connect(&m_fileinfo,
                     &FileInfo::fileChanged,
                     &m_window,
                     &Window::fileChangedHandler);
}

void Application::run()
{
    m_window.show();

    emit m_fileinfo.print(QTranslator::tr("Running: %1 %2 via Qt %3")
                          .arg(QCoreApplication::applicationName(),
                               QCoreApplication::applicationVersion(),
                               qVersion()),
                          "Application",
                          MessageLevel::Info);
}

} // namespace DialogueFromVideo
