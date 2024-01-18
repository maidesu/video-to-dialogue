#include "../../include/file/open.hpp"

namespace DialogueFromVideo::File {

Open::Open(const char* path)
    : m_formatContext(NULL)
    , m_result(0)
{

    m_result = avformat_open_input(&m_formatContext, path, NULL, NULL);
    if (m_result < 0)
    {
        avformat_close_input(&m_formatContext);
        emit print(tr("Failed to open file path!"), "File::Open", MessageLevel::Error);

        return;
    }

    emit print(tr("Successfully opened file!"), "File::Open", MessageLevel::Info);

    if (avformat_find_stream_info(m_formatContext, nullptr) < 0) {
        emit print(tr("Failed to fill in missing stream information!"), "File::Open", MessageLevel::Warning);
    }
}

Open::~Open()
{
    // >= 0 for no errors occured
    if (m_result >= 0)
    {
        avformat_close_input(&m_formatContext);
    }
}

AVStream* Open::getStream(unsigned int index)
{
    if (m_result >= 0 && index < m_formatContext->nb_streams)
    {
        return m_formatContext->streams[index];
    }

    return NULL;
}

uint Open::getStreamCount()
{
    if (m_result >= 0)
    {
        return m_formatContext->nb_streams;
    }

    return 0;
}

} // namespace DialogueFromVideo::File
