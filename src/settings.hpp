#pragma once

#include <QSettings>
#include <cstdint>

#include "common/messenger.hpp"

namespace DialogueFromVideo {

class Settings : public Messenger
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    virtual ~Settings() {};

    void loadSettings();

public slots:
    void settingsChangedHandler(int64_t usPaddingLeft,
                                int64_t usPaddingRight,
                                int64_t usMerge);

private:
    int64_t m_usPaddingLeft;
    int64_t m_usPaddingRight;
    int64_t m_usMerge;

    QSettings* m_settings;
};

} // namespace DialogueFromVideo
