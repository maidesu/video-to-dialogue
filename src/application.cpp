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

    QObject::connect(&m_settings,
                     &Settings::initialSettingsSignal,
                     &m_window,
                     &Window::initialSettingsHandler);

    QObject::connect(&m_window,
                     &Window::applySettingsSignal,
                     &m_settings,
                     &Settings::settingsChangedHandler);
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

    m_settings.loadInitialSettings();
}

} // namespace DialogueFromVideo
