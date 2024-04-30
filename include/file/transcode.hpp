#pragma once

#include <common/subtitlestructs.hpp>
#include <common/messenger.hpp>

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
}

namespace DialogueFromVideo::File {

class Transcode
{
public:
    explicit Transcode(AVFormatContext* in,
                       AVFormatContext* out,
                       AVCodecID outCodec,
                       const QList<Interval>* dialogueIntervals,
                       AVDictionary* options,
                       int targetStream);
    ~Transcode();

    Transcode(Transcode&) = delete;
    Transcode& operator= (const Transcode&) = delete;

    int getResult() { return m_result; }

private:
    AVFormatContext* m_inFormatContext;
    AVFormatContext* m_outFormatContext;
    int m_result;

    Messenger m_messenger;
};

} // namespace DialogueFromVideo::File
