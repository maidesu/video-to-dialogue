#pragma once

#include <pipe/pipe.hpp>

#include <common/singleton.hpp>

namespace DialogueFromVideo {

template <class T, class U>
class Pipeline : public IPipe<T, U>, public Singleton<Pipeline<T, U>>
{
public:
    Pipeline();
};

} // namespace DialogueFromVideo
