#include <common/formatopts.hpp>

#define __STDC_CONSTANT_MACROS

extern "C"
{
    #include <libavcodec/codec_id.h>
}

namespace DialogueFromVideo :: FormatOptions {

// https://ffmpeg.org/ffmpeg-codecs.html -> Options

QMap<QString, QList<Option>> lossyOptions()
{
    QMap<QString, QList<Option>> map;

    map["mp3"] =  { { AV_CODEC_ID_MP3, "b", "96000" },
                    { AV_CODEC_ID_MP3, "b", "128000" },
                    { AV_CODEC_ID_MP3, "b", "192000" },
                    { AV_CODEC_ID_MP3, "b", "256000" },
                    { AV_CODEC_ID_MP3, "b", "320000" } };

    map["aac"] =  { { AV_CODEC_ID_AAC, "b", "32000" },
                    { AV_CODEC_ID_AAC, "b", "48000" },
                    { AV_CODEC_ID_AAC, "b", "64000" },
                    { AV_CODEC_ID_AAC, "b", "96000" },
                    { AV_CODEC_ID_AAC, "b", "128000" },
                    { AV_CODEC_ID_AAC, "b", "160000" },
                    { AV_CODEC_ID_AAC, "b", "192000" },
                    { AV_CODEC_ID_AAC, "b", "224000" },
                    { AV_CODEC_ID_AAC, "b", "256000" },
                    { AV_CODEC_ID_AAC, "b", "320000" },
                    { AV_CODEC_ID_AAC, "b", "384000" },
                    { AV_CODEC_ID_AAC, "b", "448000" },
                    { AV_CODEC_ID_AAC, "b", "512000" } };

    // no options
    map["opus"] = { { AV_CODEC_ID_OPUS, "", "" } };

    return map;
}

QMap<QString, QList<Option>> losslessOptions()
{
    QMap<QString, QList<Option>> map;

    map["flac"] = { { AV_CODEC_ID_FLAC, "compression_level", "0" },
                    { AV_CODEC_ID_FLAC, "compression_level", "1" },
                    { AV_CODEC_ID_FLAC, "compression_level", "2" },
                    { AV_CODEC_ID_FLAC, "compression_level", "3" },
                    { AV_CODEC_ID_FLAC, "compression_level", "4" },
                    { AV_CODEC_ID_FLAC, "compression_level", "5" },
                    { AV_CODEC_ID_FLAC, "compression_level", "6" },
                    { AV_CODEC_ID_FLAC, "compression_level", "7" },
                    { AV_CODEC_ID_FLAC, "compression_level", "8" } };

    map["wav"] = { { AV_CODEC_ID_PCM_S16LE, "format", "PCM_S16LE" },
                   { AV_CODEC_ID_PCM_S24LE, "format", "PCM_S24LE" },
                   { AV_CODEC_ID_PCM_S32LE, "format", "PCM_S32LE" } };

    return map;
}

} // namespace DialogueFromVideo
