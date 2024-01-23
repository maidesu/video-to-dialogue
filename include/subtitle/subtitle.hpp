#pragma once

#include <file/read.hpp>
#include <common/messenger.hpp>

#include <QString>
#include <QList>

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
    explicit Subtitle(QObject* parent = nullptr);
    virtual ~Subtitle();

    Subtitle(const Subtitle&) = delete;
    Subtitle& operator= (const Subtitle&) = delete;

public slots:
    void subtitleRequestedHandler(File::Read* file,
                                  int selectedSubIndex,
                                  int selectedSubLayerIndex);

private:
    void clearSubs();

    QList<SubEntry*> m_subs;

signals:
    void subtitleExtractedSignal(const QList<DialogueFromVideo::SubEntry*>& subs);
};

} // namespace DialogueFromVideo
