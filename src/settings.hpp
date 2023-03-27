#pragma once

#include <cstdint>

namespace DialogueFromVideo {

class Settings
{
public:
    Settings();

private:
    int64_t m_usPaddingLeft;
    int64_t m_usPaddingRight;
    int64_t m_usMerge;
};

} // namespace DialogueFromVideo
