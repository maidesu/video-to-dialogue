#pragma once

namespace DialogueFromVideo {

template <class T>
class Singleton
{
public:
    static T& instance()
    {
        static T instance { new T{} };
        return instance;
    }

    Singleton(const Singleton& other) = delete;
    Singleton& operator=(const Singleton& other) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

protected:
    Singleton() {}
};

} // namespace DialogueFromVideo
