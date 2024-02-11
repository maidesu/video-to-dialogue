#pragma once

#include <common/messenger.hpp>

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
}

namespace DialogueFromVideo::File {

class Read
{
public:
    explicit Read(const char* path);
    ~Read();

    Read(Read&) = delete;
    Read& operator= (const Read&) = delete;


    AVFormatContext* getContext();

    AVStream* getStream(uint index);

    uint getStreamCount();

    int getResult() { return m_result; }

private:
    AVFormatContext* m_formatContext;
    int m_result;

    Messenger m_messenger;
};

} // namespace DialogueFromVideo::File
