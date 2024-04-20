#include <frame/frame.hpp>

extern "C"
{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/imgutils.h>
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

    // The AVCodecContext MUST have been opened with avcodec_open2() before packets may be fed to the decoder.
    res = avcodec_open2(avctx, decoder, NULL);

    if (res < 0)
    {
        emit m_messenger.print(tr("Critical: Failed to create codec context!"),
                               "Frame",
                               MessageLevel::Error);
        return;
    }

    // Seeking to timestamp
    res = av_seek_frame(formatCtx, selectedVideoIndex, timestamp * AV_TIME_BASE, AVSEEK_FLAG_ANY);

    if (res < 0)
    {
        emit m_messenger.print(tr("Failed to seek to timestamp!"), "Frame", MessageLevel::Error);
        return;
    }

    // Find frame
    AVFrame* frame = av_frame_alloc(); // free with av_frame_free
    AVPacket* avpkt = av_packet_alloc(); // av_packet_unref after every av_read_frame

    while (av_read_frame(formatCtx, avpkt) == 0)
    {
        if (avpkt->stream_index != selectedVideoIndex) // Skip wrong index
        {
            av_packet_unref(avpkt);
            continue;
        }

        res = avcodec_send_packet(avctx, avpkt);

        if (res < 0)
        {
            av_packet_unref(avpkt);
            continue;
        }

        res = avcodec_receive_frame(avctx, frame);

        if (res == AVERROR(EAGAIN)) // avcodec_send_packet again
        {
            av_packet_unref(avpkt);
            continue;
        }

        // Copy the keyframe to m_frameBinaryData
        if (frame->flags & AV_FRAME_FLAG_KEY) // frame->key_frame deprecated
        {
            int size = av_image_get_buffer_size(avctx->pix_fmt,
                                                avctx->width,
                                                avctx->height,
                                                1);

            m_frameBinaryData.resize(size);

            uint8_t* buff = m_frameBinaryData.data();
            av_image_copy_to_buffer(buff,
                                    size,
                                    frame->data,
                                    frame->linesize,
                                    avctx->pix_fmt,
                                    avctx->width,
                                    avctx->height,
                                    1);

            av_packet_unref(avpkt);

            av_frame_free(&frame);

            avcodec_free_context(&avctx);

            emit frameReadySignal(m_frameBinaryData,
                                  caption);

            return;
        }

        av_packet_unref(avpkt);
    }

    av_frame_free(&frame);

    avcodec_free_context(&avctx);
}

} // namespace DialogueFromVideo
