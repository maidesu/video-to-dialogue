#include <subtitle/subtitle.hpp>

#include <algorithm>

namespace DialogueFromVideo {

Subtitle::Subtitle(QObject* parent)
    : QObject(parent)
{
}

Subtitle::~Subtitle()
{
    for (SubEntry* se : m_subs)
    {
        delete se;
    }
}

void Subtitle::subtitleRequestedHandler(File::Read* file,
                                        int selectedSubIndex,
                                        int selectedSubLayerIndex)
{
    (void)selectedSubLayerIndex;

    clearSubs();

    // texi to pdf with latex
    // https://www.ffmpeg.org/doxygen/6.0/group__lavc__decoding.html

    // Retrive decoder
    const AVCodec* decoder = avcodec_find_decoder(file->getStream(selectedSubIndex)->codecpar->codec_id);

    if (!decoder)
    {
        emit m_messenger.print(tr("Unsupported subtitle codec!"),
                               "Subtitle",
                               MessageLevel::Error);
        return;
    }


    AVCodecContext* avctx = avcodec_alloc_context3(decoder); // must be freed with avcodec_free_context

    // The AVCodecContext MUST have been opened with avcodec_open2() before packets may be fed to the decoder.
    int res = avcodec_open2(avctx, decoder, NULL);

    if (res < 0)
    {
        emit m_messenger.print(tr("Critical: Failed to create codec context!"),
                               "Subtitle",
                               MessageLevel::Error);
        return;
    }

    AVFormatContext* s = file->getContext();
    AVPacket* avpkt = av_packet_alloc();
    AVSubtitle sub;

    int got_sub;

    av_seek_frame(s, selectedSubIndex, 0, AVSEEK_FLAG_ANY);

    emit m_progress.progressReset();
    emit m_progress.progressMaximum(max_progress_steps);

    int unsupported_sub = 0;

    int64_t frame_count = 0;

    constexpr int64_t some_const = 500;
    constexpr int64_t assumed_fps = 30; // not guaranteed to be provided by library
    int64_t total_est_frames = s->duration / (1000 * some_const) * assumed_fps;
    int64_t report_progress_frame = total_est_frames / max_progress_steps; // The number of frames after we update the progress by 1

    while (av_read_frame(s, avpkt) == 0)
    {
        if (avpkt->stream_index == selectedSubIndex)
        {
            avcodec_decode_subtitle2(avctx, &sub, &got_sub, avpkt);

            if (got_sub)
            {
                for (uint i = 0; i < sub.num_rects; ++i)
                {
                    AVSubtitleRect* rect = sub.rects[i];

                    const char* plain;

                    switch (rect->type)
                    {
                        default:
                        case AVSubtitleType::SUBTITLE_NONE:
                        case AVSubtitleType::SUBTITLE_BITMAP:
                            unsupported_sub = 1;
                            continue;

                        case AVSubtitleType::SUBTITLE_TEXT:
                            plain = rect->text;
                            break;

                        /*
                         * Seemingly every text-based decoder now outputs an ASS format anyways
                         * Decoder returned format:  Index, Layer, Style, Name, MarginL, MarginR, MarginV, Effect, Text
                         * ASS specification format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text
                         * Plain text is located after the 8th comma
                         */
                        case AVSubtitleType::SUBTITLE_ASS:
                            plain = rect->ass;
                            for (uint j = 0; j < 8; *plain++ == ',' ? j++ : j);
                            break;
                    }

                    m_subs.append(new SubEntry { avpkt->pts, // sub->start_display_time is relative to avpkt->pts, usually 0, so we just use the packet pts
                                                 avpkt->pts + std::max(static_cast<int64_t>(0), avpkt->duration),
                                                 QString{ plain } });
                }

                avsubtitle_free(&sub);
            }
        }

        if (frame_count % report_progress_frame == 0)
        {
            emit m_progress.progressAdd(1);
        }

        ++frame_count;

        av_packet_unref(avpkt);
    }

    avsubtitle_free(&sub);
    av_packet_free(&avpkt);
    avcodec_free_context(&avctx); // expects AVCodecContext** for some reason even though alloc returns AVCodecContext*

    if (unsupported_sub)
    {
        emit m_messenger.print(tr("Unsupported subtitles were found!"),
                               "Subtitle",
                               MessageLevel::Warning);
    }

    emit m_progress.progressComplete();

    emit m_messenger.print(tr("Processed subtitle"),
                           "Subtitle",
                           MessageLevel::Info);

    emit Subtitle::subtitleExtractedSignal(m_subs);
}

void Subtitle::clearSubs()
{
    for (SubEntry* se : m_subs)
    {
        delete se;
    }

    m_subs.clear();
}

} // namespace DialogueFromVideo
