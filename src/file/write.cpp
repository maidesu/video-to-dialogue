#include <file/write.hpp>

#include <QTranslator>

namespace DialogueFromVideo::File {

Write::Write(const char* path,
             const AVOutputFormat* oformat)
    : m_formatContext(NULL)
    , m_oformat(oformat)
    , m_result(0)
{
    if (0 > (m_result = avformat_alloc_output_context2(&m_formatContext, m_oformat, NULL, path)) )
    {
        emit m_messenger.print(QTranslator::tr("Could not open output file at specified path!"),
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
        if (m_formatContext && !(m_formatContext->flags & AVFMT_NOFILE))
        {
            avio_closep(&m_formatContext->pb);
        }

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

} // namespace DialogueFromVideo::File
