#pragma once

namespace DialogueFromVideo {

template <class T>
class Singleton
{
public:
    static T& instance()
    {
        static T instance;
        return instance;
    }

    virtual ~Singleton() {}

    Singleton(const Singleton& other) = delete;
    Singleton& operator=(const Singleton& other) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

protected:
    Singleton() {}
};

} // namespace DialogueFromVideo
