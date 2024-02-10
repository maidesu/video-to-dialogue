#include <subtitle/subtitle.hpp>

#include <algorithm>

namespace DialogueFromVideo {

Subtitle::Subtitle(QObject* parent)
    : Messenger(parent)
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
    const AVCodec* codec = avcodec_find_decoder(file->getStream(selectedSubIndex)->codecpar->codec_id);

    if (!codec)
    {
        emit print(tr("Unsupported subtitle codec!"),
                   "Subtitle",
                   MessageLevel::Error);
        return;
    }


    AVCodecContext* avctx = avcodec_alloc_context3(codec); // must be freed with avcodec_free_context

    // The AVCodecContext MUST have been opened with avcodec_open2() before packets may be fed to the decoder.
    int res = avcodec_open2(avctx, codec, NULL);

    if (res < 0)
    {
        emit print(tr("Critical: Failed to create codec context!"),
                   "Subtitle",
                   MessageLevel::Error);
        return;
    }

    int unsupported_sub = 0;

    AVFormatContext* s = file->getContext();
    AVPacket* avpkt = av_packet_alloc();
    AVSubtitle* sub = new AVSubtitle(); /* note : avsubtitle_alloc does not seem to exist
                                         * note2: undefined or null WILL crash */
    int got_sub;

    av_seek_frame(s, selectedSubIndex, 0, AVSEEK_FLAG_ANY);

    while (av_read_frame(s, avpkt) == 0)
    {
        if (avpkt->stream_index == selectedSubIndex)
        {
            avcodec_decode_subtitle2(avctx, sub, &got_sub, avpkt);

            if (got_sub)
            {
                for (uint i = 0; i < sub->num_rects; ++i)
                {
                    AVSubtitleRect* rect = sub->rects[i];

                    const char* plain;

                    switch (rect->type)
                    {
                        case AVSubtitleType::SUBTITLE_NONE:
                        case AVSubtitleType::SUBTITLE_BITMAP:
                            unsupported_sub = 1;
                            continue;

                        case AVSubtitleType::SUBTITLE_TEXT:
                            plain = rect->text;
                            break;

                        case AVSubtitleType::SUBTITLE_ASS:                      // Seemingly every text-based decoder now outputs an ASS format anyways
                            plain = rect->ass;                                  // Format: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text
                            for (uint j = 0; j < 8; *plain++ == ',' ? j++ : j); // plain text is located after the 8th comma (Layer is not present)
                            break;
                    }

                    m_subs.append(new SubEntry { avpkt->pts, // sub->start_display_time is relative to avpkt->pts, usually 0, so we just use the packet pts
                                                 avpkt->pts + std::max(static_cast<int64_t>(0), avpkt->duration),
                                                 QString{ plain } });
                }

                avsubtitle_free(sub);
            }
        }

        av_packet_unref(avpkt);
    }

    avsubtitle_free(sub);
    av_packet_unref(avpkt);
    avcodec_free_context(&avctx); // expects AVCodecContext** for some reason even though alloc returns AVCodecContext*

    if (unsupported_sub)
    {
        emit print(tr("Unsupported subtitles were found!"),
                   "Subtitle",
                   MessageLevel::Warning);
    }

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
