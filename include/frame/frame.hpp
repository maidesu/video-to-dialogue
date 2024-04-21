#pragma once

#include <file/read.hpp>
#include <common/messenger.hpp>

namespace DialogueFromVideo {

class Frame : public QObject
{
    Q_OBJECT
public:
    explicit Frame(QObject* parent = nullptr);
    virtual ~Frame();

    Frame(const Frame&) = delete;
    Frame& operator=(const Frame&) = delete;

public slots:
    void frameRequestedHandler(File::Read* file,
                               int64_t timestamp,
                               QString caption,
                               int selectedVideoIndex);

private:
    QVector<uint8_t> m_frameBinaryData;

    Messenger m_messenger;

signals:
    void frameReadySignal(const QVector<uint8_t>& frameBinaryData,
                          int width,
                          int height,
                          QString caption);

};

} // namespace DialogueFromVideo
