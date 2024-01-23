#pragma once

#include <pipe/pipe.hpp>

#include <common/singleton.hpp>

namespace DialogueFromVideo {

template <class T, class U>
class Stage : public IPipe<T, U>, public Singleton<Stage<T, U>>
{
public:
    Stage();
};

} // namespace DialogueFromVideo
