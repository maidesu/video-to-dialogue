#pragma once

#include "pipe.hpp"

namespace DialogueFromVideo {

template <class T, class U>
class Stage : public IPipe<T, U>
{
public:
    Stage();
};

} // namespace DialogueFromVideo
