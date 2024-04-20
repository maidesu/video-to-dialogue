#include <audio/audio.hpp>
#include <common/averror.hpp>

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

    const AVStream* stream = file->getStream(selectedAudioIndex);

    const AVCodec* decoder = avcodec_find_decoder(stream->codecpar->codec_id);

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

    // Progress begin
    emit m_progress.progressReset();
    emit m_progress.progressMaximum(max_progress_steps);

    int64_t frame_count = 0;
    int64_t total_est_frames = s->duration / (1000*1000) * 30;
    int64_t report_progress_frame = total_est_frames / max_progress_steps;
    // Progress end

    while (av_read_frame(s, avpkt) == 0)
    {
        if (avpkt->stream_index != selectedAudioIndex)
        {
            av_packet_unref(avpkt);
            continue;
        }

        if (0 != (res = avcodec_send_packet(avctx, avpkt)))
        {
            av_packet_unref(avpkt);
            continue;
        }

        if (0 != (res = avcodec_receive_frame(avctx, avfrm)))
        {
            av_packet_unref(avpkt);
            continue; // Call avcodec_send_packet again
        }

        AVSampleFormat sample_fmt = av_get_packed_sample_fmt(avctx->sample_fmt);

        switch (sample_fmt)
        {
            case AV_SAMPLE_FMT_DBL:
                for (int i = 0; i < avfrm->linesize[0]; i += sizeof(double))
                {
                    if (avfrm->ch_layout.nb_channels > 0)
                    {
                        // Grab first channel
                        double value = *reinterpret_cast<double*>(&avfrm->data[0][i]);
                        m_samples.append(value);
                    }
                }
                break;

            case AV_SAMPLE_FMT_FLT:
                for (int i = 0; i < avfrm->linesize[0]; i += sizeof(float))
                {
                    if (avfrm->ch_layout.nb_channels > 0)
                    {
                        float value = *reinterpret_cast<float*>(&avfrm->data[0][i]);
                        m_samples.append(value);
                    }
                }
                break;

            case AV_SAMPLE_FMT_U8:
                for (int i = 0; i < avfrm->linesize[0]; i += sizeof(uint8_t))
                {
                    if (avfrm->ch_layout.nb_channels > 0)
                    {
                        double value = *reinterpret_cast<uint8_t*>(&avfrm->data[0][i]) / static_cast<double>(UINT8_MAX);
                        m_samples.append(value);
                    }
                }
                break;

            case AV_SAMPLE_FMT_S16:
                for (int i = 0; i < avfrm->linesize[0]; i += sizeof(int16_t))
                {
                    if (avfrm->ch_layout.nb_channels > 0)
                    {
                        double value = *reinterpret_cast<int16_t*>(&avfrm->data[0][i]) / static_cast<double>(INT16_MAX);
                        m_samples.append(value);
                    }
                }
                break;

            case AV_SAMPLE_FMT_S32:
                for (int i = 0; i < avfrm->linesize[0]; i += sizeof(int32_t))
                {
                    if (avfrm->ch_layout.nb_channels > 0)
                    {
                        double value = *reinterpret_cast<int32_t*>(&avfrm->data[0][i]) / static_cast<double>(INT32_MAX);
                        m_samples.append(value);
                    }
                }
                break;

            default:
                emit m_messenger.print(tr("Sample format support not implemented!"),
                                       "Audio",
                                       MessageLevel::Error);
                return;
        }

        av_packet_unref(avpkt);

        // Progress begin
        if (frame_count % report_progress_frame == 0)
        {
            emit m_progress.progressAdd(1);
        }

        ++frame_count;
        // Progress end

    }

    av_frame_free(&avfrm);
    av_packet_free(&avpkt);
    avcodec_free_context(&avctx);

    // Progress begin
    emit m_progress.progressComplete();
    // Progress end

    emit m_messenger.print(tr("Processed audio"),
                           "Audio",
                           MessageLevel::Info);

    emit waveformReadySignal(m_samples);
}

void Audio::waveformClearHandler()
{
    clearWaveform();
}

void Audio::clearWaveform()
{
    m_samples.clear();

    m_samples.squeeze();
}

} // namespace DialogueFromVideo
