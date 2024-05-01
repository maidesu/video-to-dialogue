#include <frame/frame.hpp>

//#include <common/averror.hpp>

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/imgutils.h>
    #include <libswscale/swscale.h>
}

namespace DialogueFromVideo {

Frame::Frame(QObject* parent)
    : QObject(parent)
{
}

Frame::~Frame()
{
}

void Frame::frameRequestedHandler(File::Read* file,
                                  int64_t timestamp,
                                  QString caption,
                                  int selectedVideoIndex)
{
    m_frameBinaryData.clear();

    AVFormatContext* formatCtx = file->getContext();
    AVStream* stream = file->getStream(selectedVideoIndex);
    int res = 0;

    // Retrive decoder
    const AVCodec* decoder = avcodec_find_decoder(stream->codecpar->codec_id);

    if (!decoder)
    {
        emit m_messenger.print(tr("Unsupported video codec!"), "Frame", MessageLevel::Error);
        return;
    }

    // Allocate codec context
    AVCodecContext* avctx = avcodec_alloc_context3(decoder); // must be freed with avcodec_free_context

    res = avcodec_parameters_to_context(avctx, stream->codecpar);

    if (res < 0)
    {
        emit m_messenger.print(tr("Failed to copy codec parameters to context!"),
                               "Frame",
                               MessageLevel::Error);
        return;
    }

    // The AVCodecContext MUST have been opened with avcodec_open2() before packets may be fed to the decoder.
    res = avcodec_open2(avctx, decoder, NULL);

    if (res < 0)
    {
        emit m_messenger.print(tr("Failed to open codec context!"),
                               "Frame",
                               MessageLevel::Error);
        return;
    }

    // Seeking to timestamp - should be keyframe from no flag set
    res = av_seek_frame(formatCtx, selectedVideoIndex, timestamp, 0);

    if (res < 0)
    {
        emit m_messenger.print(tr("Failed to seek to timestamp!"),
                               "Frame",
                               MessageLevel::Warning);
        return;
    }

    // Find frame
    AVFrame* frame = av_frame_alloc(); // free with av_frame_free
    AVPacket* avpkt = av_packet_alloc(); // av_packet_unref after every av_read_frame

    while (av_read_frame(formatCtx, avpkt) >= 0)
    {
    start:
        if (avpkt->stream_index != selectedVideoIndex) // Skip wrong index
        {
            av_packet_unref(avpkt);
            continue;
        }

        res = avcodec_send_packet(avctx, avpkt);
        if (res < 0)
        {
            emit m_messenger.print(tr("Failed to send packet!"),
                                   "Frame",
                                   MessageLevel::Error);

            break;
        }

        do
        {
            res = avcodec_receive_frame(avctx, frame);

            if (res == AVERROR(EAGAIN)) // avcodec_send_packet again
            {
                av_packet_unref(avpkt);
                goto start;
            }

            if (res < 0 && res != AVERROR(EAGAIN) && res != AVERROR_EOF)
            {
                emit m_messenger.print(tr("Failed to receive frame!"),
                                       "Frame",
                                       MessageLevel::Error);

                goto end;
            }
        }
        while ( res >= 0 &&
                !(frame->flags & AV_FRAME_FLAG_KEY) ); // Receive until keyframe or out of frames
        // frame->key_frame deprecated


        // Copy the keyframe to m_frameBinaryData
        {
            // SWSCALE
            SwsContext* sws_ctx = NULL;

            sws_ctx = sws_getContext(avctx->width,  /* This function should take care of alloc */
                                     avctx->height, /* according to source code */
                                     avctx->pix_fmt,
                                     avctx->width,
                                     avctx->height,
                                     AV_PIX_FMT_RGB24,
                                     SWS_LANCZOS,
                                     NULL,
                                     NULL,
                                     NULL);

            if (NULL == sws_ctx)
            {
                emit m_messenger.print(tr("Failed to get context!"),
                                       "Frame::swscale",
                                       MessageLevel::Error);

                break;
            }

            // RGB frame
            AVFrame* rgbFrame = av_frame_alloc();

            rgbFrame->format = AV_PIX_FMT_RGB24;
            rgbFrame->width = avctx->width;
            rgbFrame->height = avctx->height;

            res = av_frame_get_buffer(rgbFrame, 0);
            if (res < 0)
            {
                emit m_messenger.print(tr("Failed to allocate frame!"),
                                       "Frame",
                                       MessageLevel::Error);

                break;
            }

            res = sws_scale(sws_ctx,
                            frame->data,
                            frame->linesize,
                            0,
                            frame->height,
                            rgbFrame->data,
                            rgbFrame->linesize);
            if (res != frame->height)
            {
                emit m_messenger.print(tr("Failed to scale image!"),
                                       "Frame::swscale",
                                       MessageLevel::Error);

                break;
            }


            // Copy result into buffer
            int size = av_image_get_buffer_size(AV_PIX_FMT_RGB24,
                                                rgbFrame->width,
                                                rgbFrame->height,
                                                1);

            m_frameBinaryData.resize(size);

            res = av_image_copy_to_buffer(m_frameBinaryData.data(),
                                          size,
                                          rgbFrame->data,
                                          rgbFrame->linesize,
                                          AV_PIX_FMT_RGB24,
                                          rgbFrame->width,
                                          rgbFrame->height,
                                          1);

            int width = rgbFrame->width;  // Set these to pass in signal before it's freed
            int height = rgbFrame->height;

            av_frame_free(&rgbFrame);

            sws_freeContext(sws_ctx);
            // SWSCALE end

            av_packet_unref(avpkt);

            av_frame_free(&frame);

            avcodec_free_context(&avctx);

            emit frameReadySignal(m_frameBinaryData,
                                  width,
                                  height,
                                  caption);

            return;
        }

        av_packet_unref(avpkt);
    }

end:
    av_frame_free(&frame);

    avcodec_free_context(&avctx);
}

} // namespace DialogueFromVideo
