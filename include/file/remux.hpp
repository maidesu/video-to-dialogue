#pragma once

#include <common/messenger.hpp>

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
}

namespace DialogueFromVideo::File {

class Remux
{
public:
    explicit Remux(AVFormatContext* in, AVFormatContext* out, int target);
    ~Remux();

    Remux(Remux&) = delete;
    Remux& operator= (const Remux&) = delete;

    int getResult() { return m_result; }

private:
    AVFormatContext* m_inFormatContext;
    AVFormatContext* m_outFormatContext;
    int m_result;

    Messenger m_messenger;
};

} // namespace DialogueFromVideo::File
