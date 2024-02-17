#include <file/remux.hpp>

#include <QTranslator>

namespace DialogueFromVideo::File {

Remux::Remux(AVFormatContext* in, AVFormatContext* out, int target)
    : m_inFormatContext(in)
    , m_outFormatContext(out)
    , m_result(0)
{
    const AVStream* inStream;
    AVStream* outStream;
    const AVCodec* encoder;

    inStream = in->streams[target];
    encoder = avcodec_find_encoder(inStream->codecpar->codec_id);

    outStream = avformat_new_stream(out, encoder); // Adds stream to context and returns pointer

    if (0 > (m_result = avcodec_parameters_copy(outStream->codecpar, inStream->codecpar)))
    {
        emit m_messenger.print(QTranslator::tr("Failed to copy codec parameters!"),
                               "File::Remux",
                               MessageLevel::Error);
        return;
    }

    // Boilerplate for fake open or something
    if (!(out->oformat->flags & AVFMT_NOFILE))
    {
        if (0 > (m_result = avio_open(&out->pb, out->url, AVIO_FLAG_WRITE)) )
        {
            emit m_messenger.print(QTranslator::tr("Could not open output file at specified path!"),
                                   "File::Remux",
                                   MessageLevel::Error);

            return;
        }
    }

    // Write header
    if (0 > (m_result = avformat_write_header(out, NULL)) )
    {
        emit m_messenger.print(QTranslator::tr("Failed to write file header!"),
                               "File::Remux",
                               MessageLevel::Error);

        return;
    }

    // Copy packets
    AVPacket* avpkt = av_packet_alloc();

    av_seek_frame(in, target, 0, AVSEEK_FLAG_ANY);

    while (av_read_frame(in, avpkt) == 0)
    {
        if (avpkt->stream_index == target)
        {
            av_packet_rescale_ts(avpkt, inStream->time_base, outStream->time_base);
            avpkt->pos = -1;
            avpkt->stream_index = 0; // IMPORTANT

            if (0 > (m_result = av_write_frame(out, avpkt)) )
            {
                emit m_messenger.print(QTranslator::tr("Error muxing packet!"),
                                       "File::Remux",
                                       MessageLevel::Error);

                return;
            }
        }
        av_packet_unref(avpkt);
    }

    av_packet_free(&avpkt);

    // Write trailer
    if (0 != (m_result = av_write_trailer(out)) )
    {
        emit m_messenger.print(QTranslator::tr("Failed to write file!"),
                               "File::Remux",
                               MessageLevel::Error);
    }
    else
    {
        emit m_messenger.print(QTranslator::tr("Successfully remuxed to: %1").arg(
                                   QString(out->url)),
                               "File::Remux",
                               MessageLevel::Info);
    }
}

Remux::~Remux()
{

}

} // namespace DialogueFromVideo::File
