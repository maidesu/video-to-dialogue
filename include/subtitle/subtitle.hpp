#pragma once

#include <file/read.hpp>
#include <common/messenger.hpp>
#include <common/progress.hpp>

#include <QString>
#include <QList>

namespace DialogueFromVideo {

struct SubEntry
{
    int64_t start;
    int64_t end;
    QString text;
};

class Subtitle : public QObject
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

    void subtitleClearHandler() { clearSubs(); } // To be used when "empty" file

private:
    void clearSubs();

    QList<SubEntry*> m_subs;

    Messenger m_messenger;
    Progress m_progress;

signals:
    void subtitleExtractedSignal(const QList<SubEntry*>& subs);
};

} // namespace DialogueFromVideo
