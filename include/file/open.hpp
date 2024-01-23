#pragma once

#include <common/messenger.hpp>

extern "C" {
#include <libavformat/avformat.h>
}

namespace DialogueFromVideo::File {

class Open : public Messenger
{
    Q_OBJECT
public:
    explicit Open(const char* path);
    ~Open();

    Open(Open&) = delete;
    Open& operator= (const Open&) = delete;

    AVStream* getStream(unsigned int index);

    unsigned int getStreamCount();

    int getResult() { return m_result; }

private:
    AVFormatContext* m_formatContext;
    int m_result;
};

} // namespace DialogueFromVideo::File
