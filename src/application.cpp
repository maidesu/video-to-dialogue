#include <application.hpp>

#include <QTranslator>
#include <QGuiApplication>

namespace DialogueFromVideo {

Application::Application()
{
    // Letting the window SpinBoxes know what the initial values are
    QObject::connect(&m_settings,
                     &Settings::initialSettingsSignal,
                     &m_window,
                     &Window::initialSettingsHandler);

    // Sending initial language to window
    QObject::connect(&m_settings,
                     &Settings::initialLanguageSignal,
                     &m_window,
                     &Window::initialLanguageHandler);

    // Sending initial color scheme to window
    QObject::connect(&m_settings,
                     &Settings::initialColorSchemeSignal,
                     &m_window,
                     &Window::initialColorSchemeHandler);

    // Send settings to Settings
    QObject::connect(&m_window,
                     &Window::applySettingsSignal,
                     &m_settings,
                     &Settings::settingsChangedHandler);

    // Window to Settings language changed
    QObject::connect(&m_window,
                     &Window::languageSettingsChangedSignal,
                     &m_settings,
                     &Settings::languageSettingsChangedHandler);

    // Window to Settings colors changed
    QObject::connect(&m_window,
                     &Window::colorSchemeSettingsChangedSignal,
                     &m_settings,
                     &Settings::colorSchemeSettingsChangedHandler);

    // Ask fileinfo to open file from window
    QObject::connect(&m_window,
                     &Window::openFileSignal,
                     &m_fileinfo,
                     &FileInfo::openFileHandler);

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

    // Currently only used to fill ComboBoxes with id's
    QObject::connect(&m_fileinfo,
                     &FileInfo::fileChanged,
                     &m_window,
                     &Window::fileChangedHandler);

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
