#pragma once

#include "include/common/messenger.hpp"

#include <QString>

namespace DialogueFromVideo {

struct SubEntry
{
    uint32_t start;
    uint32_t end;
    QString text;
};

class Subtitle : public Messenger
{
    Q_OBJECT
public:
    explicit Subtitle(QObject *parent = nullptr);
    ~Subtitle() = default;

    Subtitle(const Subtitle&) = delete;
    Subtitle& operator= (const Subtitle&) = delete;

    void extract(const char* path,
                 int selectedSubIndex,
                 int selectedSubLayerIndex);

private:
    QList<SubEntry> m_subs;
};

} // namespace DialogueFromVideo
