#include <settings.hpp>

#include <widgets/restartmessagebox.hpp>
#include <widgets/palettes/darkpalette.hpp>
#include <widgets/palettes/lightpalette.hpp>

#include <QApplication>
#include <QGuiApplication>

namespace DialogueFromVideo {

Settings::Settings(QObject *parent)
    : QObject(parent)
    , m_restartNotified(false)
    , m_usPaddingLeft(0)
    , m_usPaddingRight(0)
    , m_usOffset(0)
    , m_usMerge(0)
    , m_darkModeEnabled(false)
    , m_uiLanguage("en_US")
    , m_settings(new QSettings(QSettings::IniFormat,
                               QSettings::UserScope,
                               QGuiApplication::applicationName().replace(" ", ""),
                               "settings"))
{
    // Ui style
    QApplication::setStyle("fusion");

    m_settings->beginGroup("UX");
    m_darkModeEnabled = m_settings->value("DarkMode", "false").toBool();
    m_settings->endGroup();

    colorSchemeSettingsChangedHandler(m_darkModeEnabled);

    // Ui language settings
    m_settings->beginGroup("UX");
    m_uiLanguage = m_settings->value("Language", "en_US").toString();
    m_settings->setValue("Language", m_uiLanguage); // Write language setting asap
    m_settings->endGroup();

    // Install translator
    m_translator = new QTranslator();

    if (m_translator->load(":/i18n/" + m_uiLanguage))
    {
        QApplication::installTranslator(m_translator);
    }
    else
    {
        QApplication::quit();
    }
}

Settings::~Settings()
{
    delete m_settings;
    delete m_translator;
}

void Settings::loadInitialSettings()
{
    // Subtitle timing settings
    emit m_messenger.print(QString("Settings format: %1, Settings path: %2")
                                .arg(QString::number(m_settings->format()),
                                     m_settings->fileName()),
                           "Settings",
                           MessageLevel::Debug);

    m_settings->beginGroup("Subtitle");
    m_usPaddingLeft     = m_settings->value("PaddingLeft", 0).toLongLong();
    m_usPaddingRight    = m_settings->value("PaddingRight", 0).toLongLong();
    m_usOffset          = m_settings->value("Offset", 0).toLongLong();
    m_usMerge           = m_settings->value("Merge", 2000000).toLongLong(); // 2 seconds
    m_settings->endGroup();

    emit m_messenger.print(tr("Loaded settings: Left pad -> %1ms | Right pad -> %2ms | Offset -> %3ms | Minimum gap -> %4ms")
                                .arg(QString::number(m_usPaddingLeft / 1000),
                                     QString::number(m_usPaddingRight / 1000),
                                     QString::number(m_usOffset / 1000),
                                     QString::number(m_usMerge / 1000)),
                           "Settings",
                           MessageLevel::Info);

    emit initialSettingsSignal(m_usPaddingLeft,
                               m_usPaddingRight,
                               m_usOffset,
                               m_usMerge);

    emit initialLanguageSignal(m_uiLanguage);

    emit initialColorSchemeSignal(m_darkModeEnabled);
}

void Settings::showRestartDialog()
{
    if (!m_restartNotified)
    {
        RestartMessageBox* restartMessageBox = new RestartMessageBox();
        restartMessageBox->exec();

        m_restartNotified = true;
    }
}

void Settings::settingsChangedHandler(int64_t usPaddingLeft,
                                      int64_t usPaddingRight,
                                      int64_t usOffset,
                                      int64_t usMerge)
{
    m_usPaddingLeft     = usPaddingLeft;
    m_usPaddingRight    = usPaddingRight;
    m_usOffset          = usOffset;
    m_usMerge           = usMerge;

    m_settings->beginGroup("Subtitle");
    m_settings->setValue("PaddingLeft", m_usPaddingLeft);
    m_settings->setValue("PaddingRight", m_usPaddingRight);
    m_settings->setValue("Offset", m_usOffset);
    m_settings->setValue("Merge", m_usMerge);
    m_settings->endGroup();

    //m_settings->sync(); Normally unneeded - refer to docs

    emit m_messenger.print(tr("Updated settings: Left pad -> %1ms | Right pad -> %2ms | Offset -> %3ms | Minimum gap -> %4ms")
                                .arg(QString::number(m_usPaddingLeft / 1000),
                                     QString::number(m_usPaddingRight / 1000),
                                     QString::number(m_usOffset / 1000),
                                     QString::number(m_usMerge / 1000)),
                           "Settings",
                           MessageLevel::Info);
}

void Settings::languageSettingsChangedHandler(const QString& language)
{
    m_uiLanguage = QString(language);

    m_settings->beginGroup("UX");
    m_settings->setValue("Language", m_uiLanguage);
    m_settings->endGroup();

    showRestartDialog();
}

void Settings::colorSchemeSettingsChangedHandler(bool darkModeEnabled)
{
    m_darkModeEnabled = darkModeEnabled;

    m_settings->beginGroup("UX");
    m_settings->setValue("DarkMode", m_darkModeEnabled);
    m_settings->endGroup();

    if (!darkModeEnabled)
    {
        QApplication::setPalette(DialogueFromVideo::Palettes::createLightPalette());
    }
    else
    {
        QApplication::setPalette(DialogueFromVideo::Palettes::createDarkPalette());
    }
}

} // namespace DialogueFromVideo
