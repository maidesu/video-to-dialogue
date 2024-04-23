#include <file/transcode.hpp>

#include <QTranslator>

namespace DialogueFromVideo::File {

Transcode::Transcode(AVFormatContext* in,
                     AVFormatContext* out,
                     AVCodecID outCodec,
                     const QList<Interval>& dialogueIntervals,
                     int targetStream)
    : m_inFormatContext(in)
    , m_outFormatContext(out)
    , m_result(0)
{
    // Sanity check
    if (dialogueIntervals.isEmpty())
    {
        emit m_messenger.print(QTranslator::tr("There is nothing to transcode!"),
                               "File::Transcode",
                               MessageLevel::Error);

        return;
    }

    const AVStream* inStream = in->streams[targetStream];

    const AVCodec* decoder = avcodec_find_decoder(inStream->codecpar->codec_id);
    const AVCodec* encoder = avcodec_find_encoder(outCodec);

    if (!decoder || !encoder)
    {
        emit m_messenger.print(QTranslator::tr("Unsupported audio codec!"),
                               "File::Transcode",
                               MessageLevel::Error);
        return;
    }

    // Adds stream to context and returns pointer
    AVStream* outStream = avformat_new_stream(out, encoder);

    if (!outStream)
    {
        emit m_messenger.print(QTranslator::tr("Failed to create new stream!"),
                               "File::Transcode",
                               MessageLevel::Error);
        return;
    }

    // Context for the AVCodecs
    // must be freed with avcodec_free_context
    AVCodecContext* decoder_ctx = avcodec_alloc_context3(decoder);
    AVCodecContext* encoder_ctx = avcodec_alloc_context3(encoder);

    if (!decoder_ctx || !encoder_ctx)
    {
        emit m_messenger.print(QTranslator::tr("Failed to allocate codec context!"),
                               "File::Transcode",
                               MessageLevel::Error);
        return;
    }


    // Boilerplate for fake open or something
    if (!(out->oformat->flags & AVFMT_NOFILE))
    {
        if (0 > (m_result = avio_open(&out->pb, out->url, AVIO_FLAG_WRITE)) )
        {
            emit m_messenger.print(QTranslator::tr("Could not open output file at specified path!"),
                                   "File::Transcode",
                                   MessageLevel::Error);

            return;
        }
    }

    // Write header
    if (0 > (m_result = avformat_write_header(out, NULL)) )
    {
        emit m_messenger.print(QTranslator::tr("Failed to write file header!"),
                               "File::Transcode",
                               MessageLevel::Error);

        return;
    }

    // Transcode packets
    AVPacket* avpkt = av_packet_alloc();
    AVFrame* avfrm = av_frame_alloc();

    int intervalIdx = 0;
    int64_t start = dialogueIntervals[intervalIdx].start;
    int64_t end   = dialogueIntervals[intervalIdx].end;

    m_result = avformat_seek_file(in,
                                  targetStream,
                                  INT64_MIN,
                                  start,
                                  INT64_MAX,
                                  0);
    if (0 > m_result)
    {
        emit m_messenger.print(QTranslator::tr("Failed to seek!"),
                               "File::Transcode",
                               MessageLevel::Warning);

        return;
    }

start:
    while (av_read_frame(in, avpkt) == 0)
    {
        if (avpkt->stream_index != targetStream)
        {
            av_packet_unref(avpkt);
            continue;
        }

        if (avpkt->pts >= end)
        {
            // We need to move onto the next interval
            intervalIdx = intervalIdx + 1;

            if (intervalIdx >= dialogueIntervals.size())
            {
                // We are done
                av_packet_unref(avpkt);
                break;
            }

            start = dialogueIntervals[intervalIdx].start;
            end   = dialogueIntervals[intervalIdx].end;
        }

        if (avpkt->pts < start)
        {
            // Packet not yet at our start timing
            av_packet_unref(avpkt);
            continue;
        }

        // Decode part
        m_result = avcodec_send_packet(decoder_ctx, avpkt);

        if ( m_result != AVERROR_EOF &&
             ( m_result == AVERROR(EAGAIN) ||
               m_result < 0 ) )
        {
            av_packet_unref(avpkt);
            continue;
        }

        do
        {
            m_result = avcodec_receive_frame(decoder_ctx, avfrm);

            if (m_result == AVERROR(EAGAIN))
            {
                av_packet_unref(avpkt);
                goto start; // Call avcodec_send_packet again (with a new frame)
            }

            if (m_result == AVERROR_EOF)
            {
                av_packet_unref(avpkt);
                break;
            }
        }
        while (m_result != 0);

        // Encode part
        m_result = avcodec_send_frame(encoder_ctx, avfrm);

        av_packet_unref(avpkt);

        m_result = avcodec_receive_packet(encoder_ctx, avpkt);

        m_result = av_interleaved_write_frame(out, avpkt);

        if (m_result < 0)
        {
            emit m_messenger.print(QTranslator::tr("Failed to write packet!"),
                                   "File::Transcode",
                                   MessageLevel::Error);
            return;
        }

        av_packet_unref(avpkt);
    }

    av_frame_free(&avfrm);
    av_packet_free(&avpkt);

    // Write trailer
    if (0 != (m_result = av_write_trailer(out)) )
    {
        emit m_messenger.print(QTranslator::tr("Failed to write file!"),
                               "File::Transcode",
                               MessageLevel::Error);
    }
    else
    {
        emit m_messenger.print(QTranslator::tr("Successfully encoded to: %1").arg(
                                   QString(out->url)),
                               "File::Transcode",
                               MessageLevel::Info);
    }

    avcodec_free_context(&decoder_ctx);
    avcodec_free_context(&encoder_ctx);
}

Transcode::~Transcode()
{

}

} // namespace DialogueFromVideo::File
