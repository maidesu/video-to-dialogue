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

int64_t stringTimeToMilliseconds(const QString& string)
{
    QTime time = QTime::fromString(string, "HH:mm:ss.zzz");

    if (!time.isValid()) {
        return -1;
    }

    int64_t milliseconds = time.hour() * 3600000LL +
                           time.minute() * 60000LL +
                           time.second() * 1000LL +
                           time.msec();

    return milliseconds;
}

} // namespace DialogueFromVideo
