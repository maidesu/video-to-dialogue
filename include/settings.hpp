#pragma once

#include "common/messenger.hpp"

#include <QTranslator>
#include <QSettings>

#include <cstdint>

namespace DialogueFromVideo {

class Settings : public Messenger
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    Settings(Settings&) = delete;
    Settings(Settings&&) = delete;

    Settings& operator= (const Settings&) = delete;
    Settings& operator= (const Settings&&) = delete;

    virtual ~Settings();

    void loadInitialSettings();

public slots:
    void settingsChangedHandler(int64_t usPaddingLeft,
                                int64_t usPaddingRight,
                                int64_t usOffset,
                                int64_t usMerge);

    void languageSettingsChangedHandler(const QString& language);

private:
    int64_t m_usPaddingLeft;
    int64_t m_usPaddingRight;
    int64_t m_usOffset;
    int64_t m_usMerge;

    bool m_darkModeEnabled;
    QString m_uiLanguage;

    QSettings* m_settings;
    QTranslator* m_translator;

signals:
    void initialSettingsSignal(int64_t usPaddingLeft,
                               int64_t usPaddingRight,
                               int64_t usOffset,
                               int64_t usMerge);
};

} // namespace DialogueFromVideo
