#pragma once

namespace DialogueFromVideo {

template <class T, class U>
class IPipe
{
public:
    IPipe();
    virtual ~IPipe() = default;

    virtual void process(const T& in, U& out) = 0;
};

} // namespace DialogueFromVideo
