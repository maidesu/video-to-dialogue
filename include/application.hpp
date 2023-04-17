#pragma once

#include "fileinfo.hpp"
#include "settings.hpp"
#include "widgets/window.hpp"

namespace DialogueFromVideo {

class Application
{
public:
    Application();

    ~Application() = default;

    Application(const Application&) = delete;
    Application(const Application&&) = delete;

    Application& operator= (const Application&) = delete;
    Application& operator= (const Application&&) = delete;

    void run();

private:
    Messenger m_applicationMessenger;
    Window m_window;
    FileInfo m_fileinfo;
    Settings m_settings;
};

} // namespace DialogueFromVideo
