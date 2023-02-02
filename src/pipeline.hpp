#pragma once

#include "pipe.hpp"

namespace DialogueFromVideo {

template <class T, class U>
class Pipeline : public IPipe<T, U>
{
public:
    Pipeline();
};

} // namespace DialogueFromVideo
