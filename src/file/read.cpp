#include <file/read.hpp>

#include <QTranslator>

namespace DialogueFromVideo::File {

Read::Read(const char* path)
    : m_formatContext(NULL)
    , m_result(0)
{
    m_result = avformat_open_input(&m_formatContext, path, NULL, NULL);
    if (m_result < 0)
    {
        avformat_close_input(&m_formatContext);
        emit m_messenger.print(QTranslator::tr("Failed to open file path!"),
                               "File::Read",
                               MessageLevel::Error);

        return;
    }

    emit m_messenger.print(QTranslator::tr("Successfully opened file!"),
                           "File::Read",
                           MessageLevel::Info);

    if (avformat_find_stream_info(m_formatContext, nullptr) < 0)
    {
        emit m_messenger.print(QTranslator::tr("Failed to fill in missing stream information!"),
                               "File::Read",
                               MessageLevel::Warning);
    }
}

Read::~Read()
{
    // >= 0 for no errors occured
    if (m_result >= 0)
    {
        avformat_close_input(&m_formatContext);
    }
}

AVFormatContext* Read::getContext()
{
    if (m_result >= 0)
    {
        return m_formatContext;
    }

    return NULL;
}

AVStream* Read::getStream(uint index)
{
    if (m_result >= 0 && index < m_formatContext->nb_streams)
    {
        return m_formatContext->streams[index];
    }

    return NULL;
}

uint Read::getStreamCount()
{
    if (m_result >= 0)
    {
        return m_formatContext->nb_streams;
    }

    return 0;
}

} // namespace DialogueFromVideo::File
