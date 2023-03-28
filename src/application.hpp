#pragma once

#include "widgets/window.hpp"
#include "fileinfo.hpp"
#include "settings.hpp"
#include "common/console.hpp"

namespace DialogueFromVideo {

class Application
{
public:
    Application();
    ~Application() = default;

    void run();

private:
    Messenger m_applicationMessenger;
    Window m_window;
    FileInfo m_fileinfo;
    Settings m_settings;
    Console& m_console;
};

} // namespace DialogueFromVideo
