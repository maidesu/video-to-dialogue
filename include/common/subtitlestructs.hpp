#pragma once

#include <QString>

#include <cstdint>

namespace DialogueFromVideo {

struct Interval
{
    int64_t start;
    int64_t end;

    bool operator==(const Interval& o) const
    {
        return start == o.start && end == o.end;
    }
};

struct SubEntry
{
    int64_t start;
    int64_t end;
    QString text;

    bool operator==(const SubEntry& o) const
    {
        return start == o.start && end == o.end && text == o.text;
    }
};


} // namespace DialogueFromVideo
