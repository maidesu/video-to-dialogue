#pragma once

#include <file/read.hpp>
#include <common/messenger.hpp>
#include <common/progress.hpp>

namespace DialogueFromVideo {

class Audio : public QObject
{
    Q_OBJECT
public:
    explicit Audio(QObject* parent = nullptr);
    virtual ~Audio();

    Audio(const Audio&) = delete;
    Audio& operator= (const Audio&) = delete;

public slots:
    void waveformRequestedHandler(File::Read* file,
                                  int selectedAudioIndex);

    void waveformClearHandler();

private:
    void clearWaveform();

    QVector<double> m_samples;

    Messenger m_messenger;
    Progress m_progress;

signals:
    void waveformReadySignal(const QVector<double>& samples);
};

} // namespace DialogueFromVideo
