#include <file/transcode.hpp>
//#include <common/averror.hpp>

extern "C"
{
    #include <libswresample/swresample.h>
    #include <libavutil/opt.h>
    #include <libavutil/channel_layout.h>
    #include <libavutil/frame.h>
}

#include <QTranslator>

namespace DialogueFromVideo::File {

Transcode::Transcode(AVFormatContext* in,
                     AVFormatContext* out,
                     AVCodecID outCodec,
                     const QList<Interval>* dialogueIntervals,
                     int targetStream)
    : m_inFormatContext(in)
    , m_outFormatContext(out)
    , m_result(0)
{
    // Sanity check
    if (!dialogueIntervals || dialogueIntervals->isEmpty())
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

    // Set stuff up on the codec contexts - directly referencing transcode.c from ffmpeg examples
    // Decoder context
    m_result = avcodec_parameters_to_context(decoder_ctx, inStream->codecpar);
    if (m_result < 0)
    {
        emit m_messenger.print(QTranslator::tr("Failed to copy decoder parameters to input decoder context"),
                               "File::Transcode",
                               MessageLevel::Error);
        return;
    }

    decoder_ctx->pkt_timebase = inStream->time_base;

    m_result = avcodec_open2(decoder_ctx, decoder, NULL);
    if (m_result < 0)
    {
        emit m_messenger.print(QTranslator::tr("Failed to open decoder for stream"),
                               "File::Transcode",
                               MessageLevel::Error);
        return;
    }

    // Encoder context
    encoder_ctx->sample_rate = decoder_ctx->sample_rate;

    m_result = av_channel_layout_copy(&encoder_ctx->ch_layout,
                                      &decoder_ctx->ch_layout);

    encoder_ctx->sample_fmt = encoder->sample_fmts[0];
    encoder_ctx->time_base = AVRational{1, encoder_ctx->sample_rate};

    if (out->oformat->flags & AVFMT_GLOBALHEADER)
    {
        encoder_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    m_result = avcodec_open2(encoder_ctx, encoder, NULL);
    if (m_result < 0)
    {
        emit m_messenger.print(QTranslator::tr("Failed to open encoder for stream"),
                               "File::Transcode",
                               MessageLevel::Error);
        return;
    }

    m_result = avcodec_parameters_from_context(outStream->codecpar, encoder_ctx);
    if (m_result < 0)
    {
        emit m_messenger.print(QTranslator::tr("Failed to copy encoder parameters to output stream"),
                               "File::Transcode",
                               MessageLevel::Error);
        return;
    }

    outStream->time_base = encoder_ctx->time_base;


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
    AVFrame* avfrm_out = av_frame_alloc();
    AVFrame* avfrm_in = av_frame_alloc();

    int intervalIdx = 0;
    int64_t start = dialogueIntervals->at(intervalIdx).start;
    int64_t end   = dialogueIntervals->at(intervalIdx).end;

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

            if (intervalIdx >= dialogueIntervals->size())
            {
                // We are done
                av_packet_unref(avpkt);
                break;
            }

            start = dialogueIntervals->at(intervalIdx).start;
            end   = dialogueIntervals->at(intervalIdx).end;
        }

        if (avpkt->pts < start)
        {
            // Packet not yet at our start timing
            av_packet_unref(avpkt);
            continue;
        }

        // Decode part
        m_result = avcodec_send_packet(decoder_ctx, avpkt);

        if (m_result != 0)
        {
            av_packet_unref(avpkt);
            continue;
        }

        //do
        {
            m_result = avcodec_receive_frame(decoder_ctx, avfrm_out);

            if (m_result == AVERROR(EAGAIN) || m_result == AVERROR_EOF)
            {
                av_packet_unref(avpkt);
                goto start; // Call avcodec_send_packet again (with a new packet)
            }

            if (m_result < 0)
            {
                emit m_messenger.print(QTranslator::tr("Could not receive frame"),
                                       "File::Transcode",
                                       MessageLevel::Error);
                return;
            }

            //avfrm->pts = avfrm->best_effort_timestamp;

            /* Sample rate and channel layout should match,
             * only resample if format is different,
             * otherwise move the frame into the new frame */
            if (decoder_ctx->sample_fmt != encoder_ctx->sample_fmt)
            {
                // Sample conversion - referencing resample_audio.c
                SwrContext* swr_ctx = swr_alloc();

                // input opts
                av_opt_set_chlayout(swr_ctx,
                                    "in_chlayout",
                                    &decoder_ctx->ch_layout,
                                    0);
                av_opt_set_int(swr_ctx,
                               "in_sample_rate",
                               decoder_ctx->sample_rate,
                               0);
                av_opt_set_sample_fmt(swr_ctx,
                                      "in_sample_fmt",
                                      decoder_ctx->sample_fmt,
                                      0);

                // output opts
                av_opt_set_chlayout(swr_ctx,
                                    "out_chlayout",
                                    &encoder_ctx->ch_layout,
                                    0);
                av_opt_set_int(swr_ctx,
                               "out_sample_rate",
                               encoder_ctx->sample_rate,
                               0);
                av_opt_set_sample_fmt(swr_ctx,
                                      "out_sample_fmt",
                                      encoder_ctx->sample_fmt,
                                      0);

                m_result = swr_init(swr_ctx);
                if (m_result < 0)
                {
                    emit m_messenger.print(QTranslator::tr("Failed to initialize context!"),
                                           "File::Transcode::swresample",
                                           MessageLevel::Error);
                    return;
                }

                // Have to set these again, even though I did in av_opt_set
                avfrm_in->ch_layout = encoder_ctx->ch_layout;
                avfrm_in->sample_rate = encoder_ctx->sample_rate;
                avfrm_in->format = encoder_ctx->sample_fmt;

                m_result = swr_convert_frame(swr_ctx,  // yay i found this function
                                             avfrm_in, // which makes everything much easier
                                             avfrm_out);

                swr_free(&swr_ctx);
            }
            else
            {
                av_frame_unref(avfrm_in);

                av_frame_move_ref(avfrm_in,
                                  avfrm_out);
            }

            // Encode part
        start2:
            m_result = avcodec_send_frame(encoder_ctx, avfrm_in);

            if (m_result != 0)
            {
                av_packet_unref(avpkt);
                continue;
            }

            //do
            {
                m_result = avcodec_receive_packet(encoder_ctx, avpkt);

                if (m_result == AVERROR(EAGAIN) || m_result == AVERROR_EOF)
                {
                    av_packet_unref(avpkt);
                    goto start2; // Call avcodec_send_frame again (with a new frame)
                }

                if (m_result < 0)
                {
                    emit m_messenger.print(QTranslator::tr("Could not receive packet!"),
                                           "File::Transcode",
                                           MessageLevel::Error);
                    return;
                }

                // Prepare packet for muxing
                avpkt->stream_index = 0;
                av_packet_rescale_ts(avpkt,
                                     encoder_ctx->time_base,
                                     outStream->time_base);

                // Mux
                m_result = av_interleaved_write_frame(out, avpkt);

                if (m_result < 0)
                {
                    emit m_messenger.print(QTranslator::tr("Failed to write packet!"),
                                           "File::Transcode",
                                           MessageLevel::Error);
                    return;
                }
            }
            //while (m_result >= 0);
        }
        //while (m_result >= 0);

        av_packet_unref(avpkt);
    }

    av_frame_free(&avfrm_out);
    av_frame_free(&avfrm_in);
    av_packet_free(&avpkt);

    // Write trailer
    if (0 != (m_result = av_write_trailer(out)) )
    {
        emit m_messenger.print(QTranslator::tr("Failed to write file trailer!"),
                               "File::Transcode",
                               MessageLevel::Error);
    }
    else
    {
        emit m_messenger.print(QTranslator::tr("Successfully transcoded to: %1").arg(
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
