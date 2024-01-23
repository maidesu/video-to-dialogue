#pragma once

#include <common/messenger.hpp>

extern "C" {
#include <libavformat/avformat.h>
}

namespace DialogueFromVideo::File {

class Read : public Messenger
{
    Q_OBJECT
public:
    explicit Read(const char* path);
    ~Read();

    Read(Read&) = delete;
    Read& operator= (const Read&) = delete;

    AVStream* getStream(unsigned int index);

    unsigned int getStreamCount();

    int getResult() { return m_result; }

private:
    AVFormatContext* m_formatContext;
    int m_result;
};

} // namespace DialogueFromVideo::File
