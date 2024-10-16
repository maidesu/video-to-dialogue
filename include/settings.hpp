#pragma once

#include <common/messenger.hpp>

#include <QTranslator>
#include <QSettings>

#include <cstdint>

namespace DialogueFromVideo {

class Settings : public QObject
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

    void showRestartDialog();

public slots:
    void settingsChangedHandler(int64_t usPaddingLeft,
                                int64_t usPaddingRight,
                                int64_t usOffset,
                                int64_t usMerge);

    void languageSettingsChangedHandler(const QString& language);

    void colorSchemeSettingsChangedHandler(bool darkModeEnabled);

    void settingsRequestedHandler(int64_t& left,
                                  int64_t& right,
                                  int64_t& offset,
                                  int64_t& merge);

private:
    bool m_restartNotified;

    int64_t m_usPaddingLeft;
    int64_t m_usPaddingRight;
    int64_t m_usOffset;
    int64_t m_usMerge;

    bool m_darkModeEnabled;
    QString m_uiLanguage;

    QSettings* m_settings;
    QTranslator* m_translator;

    Messenger m_messenger;

signals:
    void initialSettingsSignal(int64_t usPaddingLeft,
                               int64_t usPaddingRight,
                               int64_t usOffset,
                               int64_t usMerge);

    void initialLanguageSignal(const QString& language);

    void initialColorSchemeSignal(bool darkModeEnabled);
};

} // namespace DialogueFromVideo
