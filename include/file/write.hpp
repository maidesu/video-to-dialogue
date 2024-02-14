#pragma once

#include <common/messenger.hpp>

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
}

namespace DialogueFromVideo::File {

class Write
{
public:
    explicit Write(const char* path);
    ~Write();

    Write(Write&) = delete;
    Write& operator= (const Write&) = delete;


    AVFormatContext* getContext();

    int writeToFile();

    int getResult() { return m_result; }

private:
    AVFormatContext* m_formatContext;
    int m_result;

    Messenger m_messenger;
};

} // namespace DialogueFromVideo::File
