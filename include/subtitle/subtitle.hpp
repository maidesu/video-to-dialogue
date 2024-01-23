#pragma once

#include <file/read.hpp>

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
    ~Subtitle();

    Subtitle(const Subtitle&) = delete;
    Subtitle& operator= (const Subtitle&) = delete;

public slots:
    void extractSubtitle(File::Read* file,
                         int selectedSubIndex,
                         int selectedSubLayerIndex);

private:
    QList<SubEntry*> m_subs;

signals:
    void subtitleExtractedSignal(const QList<DialogueFromVideo::SubEntry*>& subs);
};

} // namespace DialogueFromVideo
