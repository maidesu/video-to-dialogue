#pragma once

namespace DialogueFromVideo {

class Application
{
public:
    static Application* instance;
    Application();
    ~Application() = default;

    void run();
};

} // namespace DialogueFromVideo
