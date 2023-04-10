#pragma once

#include "common/messenger.hpp"

#include <QSettings>

#include <cstdint>

namespace DialogueFromVideo {

class Settings : public Messenger
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    virtual ~Settings() {};

    void loadInitialSettings();

public slots:
    void settingsChangedHandler(int64_t usPaddingLeft,
                                int64_t usPaddingRight,
                                int64_t usOffset,
                                int64_t usMerge);

private:
    int64_t m_usPaddingLeft;
    int64_t m_usPaddingRight;
    int64_t m_usOffset;
    int64_t m_usMerge;

    QSettings* m_settings;

signals:
    void initialSettingsSignal(int64_t usPaddingLeft,
                               int64_t usPaddingRight,
                               int64_t usOffset,
                               int64_t usMerge);
};

} // namespace DialogueFromVideo
