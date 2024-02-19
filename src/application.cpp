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


    // Ask FileManager to open file from window
    QObject::connect(&m_window,
                     &Window::openFileSignal,
                     &m_fileManager,
                     &FileManager::openFileHandler);

    // Window requests sub description from FileManager by index
    QObject::connect(&m_window,
                     &Window::subDescriptionRequestedSignal,
                     &m_fileManager,
                     &FileManager::subDescriptionRequestedHandler);

    // Window requests sub layer change from FileManager by index
    QObject::connect(&m_window,
                     &Window::subLayerRequestedSignal,
                     &m_fileManager,
                     &FileManager::subLayerRequestedHandler);

    // Window requests audio description from FileManager by index
    QObject::connect(&m_window,
                     &Window::audioDescriptionRequestedSignal,
                     &m_fileManager,
                     &FileManager::audioDescriptionRequestedHandler);

    // Window requests file processing from FileManager
    QObject::connect(&m_window,
                     &Window::processFileSignal,
                     &m_fileManager,
                     &FileManager::processFileHandler);

    // Window asks FileManager for a subtitle export
    QObject::connect(&m_window,
                     &Window::exportSubtitleSignal,
                     &m_fileManager,
                     &FileManager::exportSubtitleHandler);

    // Window ask FileManager for a picture collection export
    QObject::connect(&m_window,
                     &Window::exportPictureCollectionSignal,
                     &m_fileManager,
                     &FileManager::exportPictureCollectionHandler);

    // Window ask FileManager for a dialogue export
    QObject::connect(&m_window,
                     &Window::exportDialogueSignal,
                     &m_fileManager,
                     &FileManager::exportDialogueHandler);

    // Video remux
    QObject::connect(&m_window,
                     &Window::exportVideoRemuxSignal,
                     &m_fileManager,
                     &FileManager::exportVideoRemuxHandler);

    // Audio remux
    QObject::connect(&m_window,
                     &Window::exportAudioRemuxSignal,
                     &m_fileManager,
                     &FileManager::exportAudioRemuxHandler);

    // Subtitle remux
    QObject::connect(&m_window,
                     &Window::exportSubtitleRemuxSignal,
                     &m_fileManager,
                     &FileManager::exportSubtitleRemuxHandler);

    // Window wants to clear subs on a filechange
    QObject::connect(&m_window,
                     &Window::subtitleClearSignal,
                     &m_subtitle,
                     &Subtitle::subtitleClearHandler);


    // Currently only used to fill ComboBoxes with id's
    QObject::connect(&m_fileManager,
                     &FileManager::fileChangedSignal,
                     &m_window,
                     &Window::fileChangedHandler);

    // Send chosen sub stream info to window
    QObject::connect(&m_fileManager,
                     &FileManager::subDescriptionReceivedSignal,
                     &m_window,
                     &Window::subDescriptionReceivedHandler);

    // Send chosen audio stream info to window
    QObject::connect(&m_fileManager,
                     &FileManager::audioDescriptionReceivedSignal,
                     &m_window,
                     &Window::audioDescriptionReceivedHandler);


    // FileManager requests Subtitle after validation
    QObject::connect(&m_fileManager,
                     &FileManager::subtitleRequestedSignal,
                     &m_subtitle,
                     &Subtitle::subtitleRequestedHandler);


    // Extracted subtitle sent to window
    QObject::connect(&m_subtitle,
                     &Subtitle::subtitleExtractedSignal,
                     &m_window,
                     &Window::subtitleExtractedHandler);


    // Request waveform extraction after selected track
    QObject::connect(&m_fileManager,
                     &FileManager::waveFormRequestedSignal,
                     &m_audio,
                     &Audio::waveformRequestedHandler);

    // Extracted waveform sent to window
    QObject::connect(&m_audio,
                     &Audio::waveformReadySignal,
                     &m_window,
                     &Window::waveformReadyHandler);

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
