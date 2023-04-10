#include "application.hpp"

#include <QTranslator>
#include <QGuiApplication>

namespace DialogueFromVideo {

Application::Application()
    : m_console(Console::instance())
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

    emit m_applicationMessenger.print(QTranslator::tr("Running %1 - %2 via Qt %3")
                                            .arg(QGuiApplication::applicationDisplayName(),
                                                 QGuiApplication::applicationName(),
                                                 QGuiApplication::applicationVersion(),
                                                 qVersion()),
                                      "Application",
                                      MessageLevel::Info);

    m_settings.loadSettings();

#ifdef QT_DEBUG
    m_settings.settingsChangedHandler(1000, 1000, 1500);
#endif
}

} // namespace DialogueFromVideo
