#include <common/formatopts.hpp>

#define __STDC_CONSTANT_MACROS

extern "C"
{
    #include <libavcodec/codec_id.h>
}

namespace DialogueFromVideo :: FormatOptions {

QMap<QString, QList<Option>> lossyOptions()
{
    QMap<QString, QList<Option>> map;

    map["mp3"] =  { { AV_CODEC_ID_MP3, "bit_rate", "96" },
                    { AV_CODEC_ID_MP3, "bit_rate", "128" },
                    { AV_CODEC_ID_MP3, "bit_rate", "192" },
                    { AV_CODEC_ID_MP3, "bit_rate", "256" },
                    { AV_CODEC_ID_MP3, "bit_rate", "320" } };

    map["aac"] =  { { AV_CODEC_ID_AAC, "bit_rate", "400" } };

    map["opus"] = { { AV_CODEC_ID_OPUS, "bit_rate", "400" } };

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
