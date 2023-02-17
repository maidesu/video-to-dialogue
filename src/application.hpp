#pragma once

#include "common/console.hpp"
#include "widgets/window.hpp"

namespace DialogueFromVideo {

class Application
{
public:
    //static Application* instance;
    Application();
    ~Application() = default;

    void run();

private:
    Window m_window;
    Console const& m_console;
};

} // namespace DialogueFromVideo
