#pragma once

#include <file/open.hpp>

#include <QString>
#include <QList>

namespace DialogueFromVideo {

struct SubEntry
{
    uint32_t start;
    uint32_t end;
    QString text;
};

class Subtitle
{
public:
    explicit Subtitle() = default;
    ~Subtitle() = default;

    Subtitle(const Subtitle&) = delete;
    Subtitle& operator= (const Subtitle&) = delete;

    bool extractSubtitle();

private:
    QList<SubEntry> m_subs;
    File::Open* m_file;

signals:
    void subtitleExtractedSignal(); // TODO signal
};

} // namespace DialogueFromVideo
