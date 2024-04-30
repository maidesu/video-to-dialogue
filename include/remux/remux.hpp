#pragma once

#include <common/messenger.hpp>
#include <common/progress.hpp>

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
}

namespace DialogueFromVideo {

class Remux
{
public:
    explicit Remux(AVFormatContext* in,
                   AVFormatContext* out,
                   int target);
    ~Remux();

    Remux(Remux&) = delete;
    Remux& operator= (const Remux&) = delete;

    int getResult() { return m_result; }

private:
    AVFormatContext* m_inFormatContext;
    AVFormatContext* m_outFormatContext;
    int m_result;

    Messenger m_messenger;
    Progress m_progress;
};

} // namespace DialogueFromVideo
