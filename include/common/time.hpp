#pragma once

#include <QString>
#include <QTime>

namespace DialogueFromVideo :: Time {

QString millisecondsToStringTime(int64_t ms)
{
    QTime time(0, 0, 0, 0);
    time = time.addMSecs(ms); // "Note that the time will wrap if it passes midnight."

    return time.toString("HH:mm:ss.zzz");
}

} // namespace DialogueFromVideo
