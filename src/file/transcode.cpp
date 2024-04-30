#include <file/transcode.hpp>
//#include <common/averror.hpp>

extern "C"
{
    #include <libswresample/swresample.h>
    #include <libavutil/opt.h>
    #include <libavutil/channel_layout.h>
    #include <libavutil/frame.h>
    #include <libavfilter/avfilter.h>
    #include <libavfilter/buffersrc.h>
    #include <libavfilter/buffersink.h>
}

#include <QTranslator>

namespace DialogueFromVideo::File {

Transcode::Transcode(AVFormatContext* in,
                     AVFormatContext* out,
                     AVCodecID outCodec,
                     const QList<Interval>* dialogueIntervals,
                     AVDictionary* options,
                     int targetStream)
    : m_inFormatContext(in)
    , m_outFormatContext(out)
    , m_result(0)
{
    //av_log_set_level(AV_LOG_DEBUG);

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

        avcodec_free_context(&decoder_ctx);
        avcodec_free_context(&encoder_ctx);

        return;
    }

    decoder_ctx->pkt_timebase = inStream->time_base;

    m_result = avcodec_open2(decoder_ctx, decoder, NULL);
    if (m_result < 0)
    {
        emit m_messenger.print(QTranslator::tr("Failed to open decoder for stream"),
                               "File::Transcode",
                               MessageLevel::Error);

        avcodec_free_context(&decoder_ctx);
        avcodec_free_context(&encoder_ctx);

        return;
    }

    // Encoder context
    encoder_ctx->sample_rate = decoder_ctx->sample_rate;

    m_result = av_channel_layout_copy(&encoder_ctx->ch_layout,
                                      &decoder_ctx->ch_layout);

    encoder_ctx->sample_fmt = encoder->sample_fmts[0];
    encoder_ctx->time_base = AVRational{1, encoder_ctx->sample_rate};

    //encoder_ctx->bit_rate = 0; // FLAC supposedly doesn't care, only lossy encoders
    //encoder_ctx->frame_size = avfrm_in->nb_samples; // Doesn't work, need filters to get correctly sized frames


    if (out->oformat->flags & AVFMT_GLOBALHEADER)
    {
        encoder_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    m_result = avcodec_open2(encoder_ctx, encoder, &options);
    if (m_result < 0)
    {
        emit m_messenger.print(QTranslator::tr("Failed to open encoder for stream"),
                               "File::Transcode",
                               MessageLevel::Error);

        avcodec_free_context(&decoder_ctx);
        avcodec_free_context(&encoder_ctx);

        return;
    }

    m_result = avcodec_parameters_from_context(outStream->codecpar, encoder_ctx);
    if (m_result < 0)
    {
        emit m_messenger.print(QTranslator::tr("Failed to copy encoder parameters to output stream"),
                               "File::Transcode",
                               MessageLevel::Error);

        avcodec_free_context(&decoder_ctx);
        avcodec_free_context(&encoder_ctx);

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

            avcodec_free_context(&decoder_ctx);
            avcodec_free_context(&encoder_ctx);

            return;
        }
    }

    // Write header
    if (0 > (m_result = avformat_write_header(out, NULL)) )
    {
        emit m_messenger.print(QTranslator::tr("Failed to write file header!"),
                               "File::Transcode",
                               MessageLevel::Error);

        avcodec_free_context(&decoder_ctx);
        avcodec_free_context(&encoder_ctx);

        return;
    }

    // Create filter

    // https://stackoverflow.com/questions/16904841/how-to-encode-resampled-pcm-audio-to-aac-using-ffmpeg-api-when-input-pcm-samples
    // "just use asetnsamples"

    // Using example: https://ffmpeg.org/doxygen/6.0/filter_audio_8c-example.html
    // Filter list: https://ffmpeg.org/ffmpeg-filters.html

    // Filter
    AVFilterGraph* filter_graph = avfilter_graph_alloc();
    if (!filter_graph)
    {
        emit m_messenger.print(QTranslator::tr("Failed to allocate filter graph!"),
                               "File::Transcode::avfilter",
                               MessageLevel::Error);

        avcodec_free_context(&decoder_ctx);
        avcodec_free_context(&encoder_ctx);

        return;
    }

    const AVFilter* filter = avfilter_get_by_name("asetnsamples");
    const AVFilter* buffer = avfilter_get_by_name("abuffer");
    const AVFilter* bufferSink = avfilter_get_by_name("abuffersink");

    if (!filter || !buffer || !bufferSink)
    {
        emit m_messenger.print(QTranslator::tr("Failed to find required filter!"),
                               "File::Transcode::avfilter",
                               MessageLevel::Error);

        avcodec_free_context(&decoder_ctx);
        avcodec_free_context(&encoder_ctx);

        avfilter_graph_free(&filter_graph);

        return;
    }

    AVFilterContext* filter_ctx = avfilter_graph_alloc_filter(filter_graph, filter, "nsamples");
    AVFilterContext* buffer_ctx = avfilter_graph_alloc_filter(filter_graph, buffer, "src");
    AVFilterContext* bufferSink_ctx = avfilter_graph_alloc_filter(filter_graph, bufferSink, "sink");

    if (!filter_ctx || !buffer_ctx || !bufferSink_ctx)
    {
        emit m_messenger.print(QTranslator::tr("Failed to allocate filter context!"),
                               "File::Transcode::avfilter",
                               MessageLevel::Error);

        avcodec_free_context(&decoder_ctx);
        avcodec_free_context(&encoder_ctx);

        avfilter_graph_free(&filter_graph);

        return;
    }

    av_opt_set_int(filter_ctx,
                   "nb_out_samples",
                   encoder_ctx->frame_size ? encoder_ctx->frame_size : 1024,
                   AV_OPT_SEARCH_CHILDREN);
    //av_opt_set(filter_ctx,
    //           "nb_out_samples",
    //           std::to_string(encoder_ctx->frame_size).c_str(),
    //           AV_OPT_SEARCH_CHILDREN);
    //m_result = avfilter_init_str(filter_ctx, "n=1024:p=0");

    char ch_layout[64];
    av_channel_layout_describe(&encoder_ctx->ch_layout,
                               ch_layout,
                               sizeof(ch_layout));

    av_opt_set(buffer_ctx,
               "channel_layout",
               ch_layout,
               AV_OPT_SEARCH_CHILDREN);
    av_opt_set_int(buffer_ctx,
                   "sample_fmt",
                   encoder_ctx->sample_fmt,
                   AV_OPT_SEARCH_CHILDREN);
    av_opt_set_q(buffer_ctx,
                 "time_base",
                 encoder_ctx->time_base,
                 AV_OPT_SEARCH_CHILDREN);
    av_opt_set_int(buffer_ctx,
                   "sample_rate",
                   encoder_ctx->sample_rate,
                   AV_OPT_SEARCH_CHILDREN);

    if ( avfilter_init_str(filter_ctx, NULL) ||
        avfilter_init_str(buffer_ctx, NULL) ||
        avfilter_init_str(bufferSink_ctx, NULL) )
    {
        emit m_messenger.print(QTranslator::tr("Failed to initialize filter context!"),
                               "File::Transcode::avfilter",
                               MessageLevel::Error);

        avcodec_free_context(&decoder_ctx);
        avcodec_free_context(&encoder_ctx);

        avfilter_free(filter_ctx);
        avfilter_free(buffer_ctx);
        avfilter_free(bufferSink_ctx);

        avfilter_graph_free(&filter_graph);

        return;
    }

    m_result = avfilter_link(buffer_ctx, 0, filter_ctx, 0);
    if (!m_result)
    {
        m_result = avfilter_link(filter_ctx, 0, bufferSink_ctx, 0);
    }
    if (m_result)
    {
        emit m_messenger.print(QTranslator::tr("Failed to connect filters!"),
                               "File::Transcode::avfilter",
                               MessageLevel::Error);

        avcodec_free_context(&decoder_ctx);
        avcodec_free_context(&encoder_ctx);

        avfilter_free(filter_ctx);
        avfilter_free(buffer_ctx);
        avfilter_free(bufferSink_ctx);

        avfilter_graph_free(&filter_graph);

        return;
    }

    m_result = avfilter_graph_config(filter_graph, NULL);
    if (m_result < 0) {
        emit m_messenger.print(QTranslator::tr("Failed to configure filter graph!"),
                               "File::Transcode::avfilter",
                               MessageLevel::Error);

        avcodec_free_context(&decoder_ctx);
        avcodec_free_context(&encoder_ctx);

        avfilter_free(filter_ctx);
        avfilter_free(buffer_ctx);
        avfilter_free(bufferSink_ctx);

        avfilter_graph_free(&filter_graph);

        return;
    }


    // Sample conversion init - referencing resample_audio.c
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

        avcodec_free_context(&decoder_ctx);
        avcodec_free_context(&encoder_ctx);

        avfilter_free(filter_ctx);
        avfilter_free(buffer_ctx);
        avfilter_free(bufferSink_ctx);

        avfilter_graph_free(&filter_graph);

        return;
    }


    // Transcode packets
    AVPacket* avpkt_out = av_packet_alloc();
    AVPacket* avpkt_in = av_packet_alloc();
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

    // Manually settings pts for accurate stream duration
    //int64_t pts = 0;

start:
    while (av_read_frame(in, avpkt_out) == 0)
    {
        if (avpkt_out->stream_index != targetStream)
        {
            // Go next (until matching stream index)
            av_frame_unref(avfrm_out);
            av_frame_unref(avfrm_in);
            av_packet_unref(avpkt_out);
            continue;
        }

        if (avpkt_out->pts >= end)
        {
            // We need to move onto the next interval
            intervalIdx = intervalIdx + 1;

            if (intervalIdx >= dialogueIntervals->size())
            {
                // We are done
                break;
            }

            start = dialogueIntervals->at(intervalIdx).start;
            end   = dialogueIntervals->at(intervalIdx).end;
        }

        if (avpkt_out->pts < start)
        {
            // Packet not yet at our start timing
            av_frame_unref(avfrm_out);
            av_frame_unref(avfrm_in);
            av_packet_unref(avpkt_out);
            continue;
        }

        // Decode part
        m_result = avcodec_send_packet(decoder_ctx, avpkt_out);

        if (m_result != 0)
        {
            emit m_messenger.print(QTranslator::tr("Failed to send packet!"),
                                   "File::Transcode",
                                   MessageLevel::Error);

            goto end;
        }

        do
        {
            m_result = avcodec_receive_frame(decoder_ctx, avfrm_out);

            if (m_result == AVERROR(EAGAIN))
            {
                av_frame_unref(avfrm_out);
                av_frame_unref(avfrm_in);
                av_packet_unref(avpkt_out);
                goto start; // Call avcodec_send_packet again (with a new packet)
            }

            if (m_result < 0)
            {
                emit m_messenger.print(QTranslator::tr("Failed to receive frame!"),
                                       "File::Transcode",
                                       MessageLevel::Error);

                goto end;
            }

            //avfrm->pts = avfrm->best_effort_timestamp;

            // Resample

            /* Sample rate and channel layout should match,
             * only resample if format is different,
             * otherwise move the frame into the new frame
             * note: always resample to avoid weird errors */
            if (true || decoder_ctx->sample_fmt != encoder_ctx->sample_fmt)
            {
                avfrm_in->ch_layout = encoder_ctx->ch_layout;
                avfrm_in->sample_rate = encoder_ctx->sample_rate;
                avfrm_in->format = encoder_ctx->sample_fmt;
                //avfrm_in->nb_samples = encoder_ctx->frame_size;

                m_result = swr_convert_frame(swr_ctx,  // yay i found this function
                                             avfrm_in, // which makes everything much easier
                                             avfrm_out);

                if (m_result < 0)
                {
                    emit m_messenger.print(QTranslator::tr("Failed to resample frame!"),
                                           "File::Transcode::swresample",
                                           MessageLevel::Error);

                    goto end;
                }
            }
            else
            {
                av_frame_unref(avfrm_in);

                av_frame_move_ref(avfrm_in,
                                  avfrm_out);
            }

            // Filter

            m_result = av_buffersrc_add_frame(buffer_ctx, avfrm_in);
            if (m_result < 0)
            {
                emit m_messenger.print(QTranslator::tr("Failed to send frame to filter!"),
                                       "File::Transcode::avfilter",
                                       MessageLevel::Error);

                goto end;
            }

            //avfrm_in->pkt_dts = avfrm_in->pts = pts;
            //pts = pts + avfrm_in->duration;
            //avfrm_in->time_base = encoder_ctx->time_base;

            av_frame_unref(avfrm_out);
            av_frame_unref(avfrm_in);
        }
        while (m_result >= 0);

        av_frame_unref(avfrm_out);
        av_frame_unref(avfrm_in);
        av_packet_unref(avpkt_out);
    }

start2:
    while (av_buffersink_get_frame(bufferSink_ctx, avfrm_in) >= 0)
    {
        m_result = avcodec_send_frame(encoder_ctx, avfrm_in);

        if (m_result == AVERROR(EINVAL))
        {
            //int isOpen = avcodec_is_open(encoder_ctx); (void)isopen;
            //int isEncoder = av_codec_is_encoder(encoder_ctx->codec); (void)isencoder;

            /* The encoder wants to flush (BUT WHY???)
             *
             * spoiler: encoder_ctx->frame_size = avfrm_in->nb_samples;
             * encoder enters flush by itself when a frame is smaller than frame_size, aka. not the exact same
             *
             * https://medium.com/@restlessladder/real-time-h-265-video-stream-encoding-with-c-and-libavcodec-77eafc20fa9e
             * "AVERROR(EINVAL) — Codec not opened, refcounted_frames not set, it is a decoder, or __requires flush__."
             * https://ffmpeg.org/doxygen/6.0/group__lavc__decoding.html#ga9395cb802a5febf1f00df31497779169
             * AVERROR(EINVAL) codec not opened, it is a decoder, or requires flush */

            //m_result = avcodec_send_frame(encoder_ctx, NULL);

            emit m_messenger.print(QTranslator::tr("Encoder bad argument!"),
                                   "File::Transcode",
                                   MessageLevel::Error);

            goto end;
        }

        if (m_result != 0)
        {
            emit m_messenger.print(QTranslator::tr("Failed to send frame!"),
                                   "File::Transcode",
                                   MessageLevel::Error);

            goto end;
        }

        //int eof = 0;

        do
        {
            m_result = avcodec_receive_packet(encoder_ctx, avpkt_in);

            //if (m_result == AVERROR_EOF)
            //{
            //    avcodec_flush_buffers(encoder_ctx);
            //    eof = 1;
            //}

            if (m_result == AVERROR(EAGAIN))
            {
                av_frame_unref(avfrm_in);
                av_packet_unref(avpkt_in);
                goto start2; // Call avcodec_send_frame again (with a new frame)
            }

            if (m_result < 0 && m_result != AVERROR(EAGAIN) && m_result != AVERROR_EOF)
            {
                emit m_messenger.print(QTranslator::tr("Failed to receive packet!"),
                                       "File::Transcode",
                                       MessageLevel::Error);

                goto end;
            }

            // Prepare packet for muxing
            avpkt_in->stream_index = 0;
            av_packet_rescale_ts(avpkt_in,
                                 encoder_ctx->time_base,
                                 outStream->time_base);

            // Mux
            m_result = av_interleaved_write_frame(out, avpkt_in);
            if (m_result < 0)
            {
                emit m_messenger.print(QTranslator::tr("Failed to write packet!"),
                                       "File::Transcode",
                                       MessageLevel::Error);

                goto end;
            }

            av_packet_unref(avpkt_in);
        }
        while (m_result >= 0);// && !eof);

        av_frame_unref(avfrm_in);
        av_packet_unref(avpkt_in);
    }

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

end:
    // encode/decode
    av_frame_free(&avfrm_out);
    av_frame_free(&avfrm_in);
    av_packet_free(&avpkt_out);
    av_packet_free(&avpkt_in);

    avcodec_free_context(&decoder_ctx);
    avcodec_free_context(&encoder_ctx);

    // filter
    avfilter_free(filter_ctx);
    avfilter_free(buffer_ctx);
    avfilter_free(bufferSink_ctx);

    avfilter_graph_free(&filter_graph);

    // resampler
    swr_free(&swr_ctx);

    // note: format context free is handled by write/read wrapper
}

Transcode::~Transcode()
{

}

} // namespace DialogueFromVideo::File
