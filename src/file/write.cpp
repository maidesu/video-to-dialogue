#include <file/write.hpp>

#include <QTranslator>

namespace DialogueFromVideo::File {

Write::Write(const char* path)
    : m_formatContext(NULL)
    , m_result(0)
{
    m_result = avformat_alloc_output_context2(&m_formatContext, NULL, NULL, path);
    if (m_result < 0)
    {
        emit m_messenger.print(QTranslator::tr("Failed to save at specified path!"),
                               "File::Write",
                               MessageLevel::Error);

        return;
    }
}

Write::~Write()
{
    // >= 0 for no errors occured
    if (m_result >= 0)
    {
        avformat_free_context(m_formatContext);
    }
}

AVFormatContext* Write::getContext()
{
    if (m_result >= 0)
    {
        return m_formatContext;
    }

    return NULL;
}

int Write::writeToFile()
{
    int res;

    if (0 != (res = av_write_trailer(m_formatContext)))
    {
        emit m_messenger.print(QTranslator::tr("Failed to write file!"),
                               "File::Write",
                               MessageLevel::Error);
    }
    else
    {
        emit m_messenger.print(QTranslator::tr("Successfully written to path: %1").arg(
                                   QString(m_formatContext->url)),
                               "File::Write",
                               MessageLevel::Info);
    }

    return res;
}

} // namespace DialogueFromVideo::File
