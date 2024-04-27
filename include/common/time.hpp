#pragma once

#include <QString>

#include <cstdint>

namespace DialogueFromVideo :: Time {

QString millisecondsToStringTime(int64_t ms);

int64_t stringTimeToMilliseconds(const QString& string);

} // namespace DialogueFromVideo
