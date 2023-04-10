#include "../include/settings.hpp"

#include <QGuiApplication>

namespace DialogueFromVideo {

Settings::Settings(QObject *parent)
    : Messenger(parent)
    , m_usPaddingLeft(0)
    , m_usPaddingRight(0)
    , m_usOffset(0)
    , m_usMerge(0)
    , m_settings(new QSettings(QSettings::IniFormat,
                               QSettings::UserScope,
                               QGuiApplication::applicationName().replace(" ", ""),
                               "settings"))
{

}

void Settings::loadInitialSettings()
{
    emit print(tr("Settings format: %1, Settings path: %2")
                    .arg(QString::number(m_settings->format()),
                         m_settings->fileName()),
               "Settings",
               MessageLevel::Debug);

    m_usPaddingLeft     = m_settings->value("PaddingLeft", 0).toLongLong();
    m_usPaddingRight    = m_settings->value("PaddingRight", 0).toLongLong();
    m_usOffset          = m_settings->value("Offset", 0).toLongLong();
    m_usMerge           = m_settings->value("Merge", 2000000).toLongLong(); // 2 seconds

    emit print(tr("Loaded settings: Left pad -> %1ms | Right pad -> %2ms | Offset -> %3ms | Minimum gap -> %4ms")
                    .arg(QString::number(m_usPaddingLeft/1000),
                         QString::number(m_usPaddingRight/1000),
                         QString::number(m_usOffset/1000),
                         QString::number(m_usMerge/1000)),
               "Settings",
               MessageLevel::Info);

    emit initialSettingsSignal(m_usPaddingLeft,
                               m_usPaddingRight,
                               m_usOffset,
                               m_usMerge);
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

    m_settings->setValue("PaddingLeft", m_usPaddingLeft);
    m_settings->setValue("PaddingRight", m_usPaddingRight);
    m_settings->setValue("Offset", m_usOffset);
    m_settings->setValue("Merge", m_usMerge);

    //m_settings->sync(); Normally unneeded - refer to docs

    emit print(tr("Updated settings: Left pad -> %1ms | Right pad -> %2ms | Offset -> %3ms | Minimum gap -> %4ms")
                    .arg(QString::number(m_usPaddingLeft/1000),
                         QString::number(m_usPaddingRight/1000),
                         QString::number(m_usOffset/1000),
                         QString::number(m_usMerge/1000)),
               "Settings",
               MessageLevel::Info);
}

} // namespace DialogueFromVideo
