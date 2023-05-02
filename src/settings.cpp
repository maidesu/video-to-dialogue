#include "../include/settings.hpp"

#include <QApplication>
#include <QGuiApplication>
#include <QProcess>

namespace DialogueFromVideo {

Settings::Settings(QObject *parent)
    : Messenger(parent)
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
    emit print(QString("Settings format: %1, Settings path: %2")
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

    emit print(tr("Loaded settings: Left pad -> %1ms | Right pad -> %2ms | Offset -> %3ms | Minimum gap -> %4ms")
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

    emit print(tr("Updated settings: Left pad -> %1ms | Right pad -> %2ms | Offset -> %3ms | Minimum gap -> %4ms")
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

    // TODO Dialogue window restart
    QApplication::quit();
    //QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void Settings::colorSchemeSettingsChangedHandler(bool darkModeEnabled)
{
    m_darkModeEnabled = darkModeEnabled;

    m_settings->beginGroup("UX");
    m_settings->setValue("DarkMode", m_darkModeEnabled);
    m_settings->endGroup();

    // TODO
}

} // namespace DialogueFromVideo
