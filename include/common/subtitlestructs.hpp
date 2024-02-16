#pragma once

#include <QString>

#include <stdint.h>

namespace DialogueFromVideo {

struct Interval
{
    int64_t start;
    int64_t end;
};

struct SubEntry
{
    int64_t start;
    int64_t end;
    QString text;
};


} // namespace DialogueFromVideo
