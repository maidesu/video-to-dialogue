#include <subtitle/subtitle.hpp>

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
    assert(res == 0);


    AVFormatContext* s = file->getContext();
    AVPacket* avpkt = av_packet_alloc();
    AVSubtitle* sub = new AVSubtitle(); /* note : avsubtitle_alloc does not seem to exist
                                         * note2: undefined or null WILL crash */
    int got_sub;

    while (av_read_frame(s, avpkt) == 0)
    {
        if (avpkt->stream_index == selectedSubIndex)
        {
            avcodec_decode_subtitle2(avctx, sub, &got_sub, avpkt);

            if (got_sub)
            {
                // Access the plain text from subtitle.rects
                for (unsigned int i = 0; i < sub->num_rects; ++i)
                {
                    AVSubtitleRect* rect = sub->rects[i];
                    // rect->text contains the plain text
                    (void)rect; // TODO: if rect->ass (formatted) then rect->text is null
                }

                avsubtitle_free(sub);
            }
        }

        av_packet_unref(avpkt);
    }

    avcodec_free_context(&avctx); // expects AVCodecContext** for some reason even though alloc returns AVCodecContext*

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
