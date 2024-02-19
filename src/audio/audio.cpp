#include <audio/audio.hpp>

namespace DialogueFromVideo {

Audio::Audio(QObject* parent)
    : QObject(parent)
{
}

Audio::~Audio()
{
}

void Audio::waveformRequestedHandler(File::Read* file,
                                     int selectedAudioIndex)
{
    clearWaveform();

    const AVCodec* decoder = avcodec_find_decoder(file->getStream(selectedAudioIndex)->codecpar->codec_id);

    if (!decoder)
    {
        emit m_messenger.print(tr("Unsupported audio codec!"),
                               "Audio",
                               MessageLevel::Error);
        return;
    }


    AVCodecContext* avctx = avcodec_alloc_context3(decoder); // must be freed with avcodec_free_context

    // The AVCodecContext MUST have been opened with avcodec_open2() before packets may be fed to the decoder.
    int res = avcodec_open2(avctx, decoder, NULL);

    if (res < 0)
    {
        emit m_messenger.print(tr("Critical: Failed to create codec context!"),
                               "Audio",
                               MessageLevel::Error);
        return;
    }

    AVFormatContext* s = file->getContext();
    AVPacket* avpkt = av_packet_alloc();
    AVFrame* avfrm = av_frame_alloc();

    av_seek_frame(s, selectedAudioIndex, 0, AVSEEK_FLAG_ANY);

    while (av_read_frame(s, avpkt) == 0)
    {
        if (avpkt->stream_index != selectedAudioIndex)
        {
            av_packet_unref(avpkt);
            continue;
        }

        if (0 != (res = avcodec_send_packet(avctx, avpkt)))
        {
            //char buff[256];
            //av_strerror(res, buff, 256);

            continue;
        }

        if (0 != (res = avcodec_receive_frame(avctx, avfrm)))
        {
            //char buff[256];
            //av_strerror(res, buff, 256);

            continue;
        }

        AVSampleFormat sample_fmt = av_get_packed_sample_fmt(avctx->sample_fmt);

        if (sample_fmt == AV_SAMPLE_FMT_DBL)
        {
            for (int i = 0; i < avfrm->linesize[0]; i += sizeof(double))
            {
                if (avfrm->ch_layout.nb_channels > 0)
                {
                    // Grab first channel
                    double value = *reinterpret_cast<double*>(&avfrm->data[0][i]);
                    m_samples.append(value);
                }
            }
        }
        else if (sample_fmt == AV_SAMPLE_FMT_FLT)
        {
            for (int i = 0; i < avfrm->linesize[0]; i += sizeof(float))
            {
                if (avfrm->ch_layout.nb_channels > 0)
                {
                    // Grab first channel
                    float value = *reinterpret_cast<float*>(&avfrm->data[0][i]);
                    m_samples.append(value);
                }
            }
        }
        else
        {
            emit m_messenger.print(tr("Sample format support not implemented!"),
                                   "Audio",
                                   MessageLevel::Error);
            break;
        }

        av_packet_unref(avpkt);
    }

    av_frame_free(&avfrm);
    av_packet_free(&avpkt);
    avcodec_free_context(&avctx);

    emit waveformReadySignal(m_samples);
}

void Audio::waveformClearHandler()
{
    clearWaveform();
}

void Audio::clearWaveform()
{
    m_samples.clear();
}

} // namespace DialogueFromVideo
