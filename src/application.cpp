#include "../include/application.hpp"

#include <QTranslator>
#include <QGuiApplication>

namespace DialogueFromVideo {

Application::Application()
{
    // Ask fileinfo to open file from window
    QObject::connect(&m_window,
                     &Window::openFileSignal,
                     &m_fileinfo,
                     &FileInfo::openFileHandler);

    // Currently only used to fill ComboBoxes with id's
    QObject::connect(&m_fileinfo,
                     &FileInfo::fileChanged,
                     &m_window,
                     &Window::fileChangedHandler);

    // Letting the window SpinBoxes know what the initial values are
    QObject::connect(&m_settings,
                     &Settings::initialSettingsSignal,
                     &m_window,
                     &Window::initialSettingsHandler);

    // Send settings to Settings
    QObject::connect(&m_window,
                     &Window::applySettingsSignal,
                     &m_settings,
                     &Settings::settingsChangedHandler);

    // Window requests sub description from FileInfo by index
    QObject::connect(&m_window,
                     &Window::subDescriptionRequestedSignal,
                     &m_fileinfo,
                     &FileInfo::subDescriptionRequestedHandler);

    // Window requests sub layer change from FileInfo by index
    QObject::connect(&m_window,
                     &Window::subLayerRequestedSignal,
                     &m_fileinfo,
                     &FileInfo::subLayerRequestedHandler);

    // Window requests audio description from FileInfo by index
    QObject::connect(&m_window,
                     &Window::audioDescriptionRequestedSignal,
                     &m_fileinfo,
                     &FileInfo::audioDescriptionRequestedHandler);

    // Send chosen sub stream info to window
    QObject::connect(&m_fileinfo,
                     &FileInfo::subDescriptionReceivedSignal,
                     &m_window,
                     &Window::subDescriptionReceivedHandler);

    // Send chosen audio stream info to window
    QObject::connect(&m_fileinfo,
                     &FileInfo::audioDescriptionReceivedSignal,
                     &m_window,
                     &Window::audioDescriptionReceivedHandler);
}

void Application::run()
{
    m_settings.foo();

    m_window.show();

    emit m_applicationMessenger.print(QTranslator::tr("Running %1 - %2 %3 via Qt %4")
                                            .arg(QGuiApplication::applicationDisplayName(),
                                                 QGuiApplication::applicationName(),
                                                 QGuiApplication::applicationVersion(),
                                                 qVersion()),
                                      "Application",
                                      MessageLevel::Info);

    m_settings.loadInitialSettings();
}

} // namespace DialogueFromVideo
