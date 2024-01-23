#pragma once

#include <fileinfo.hpp>
#include <settings.hpp>
#include <subtitle/subtitle.hpp>
#include <widgets/window.hpp>

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
    Settings m_settings;
    FileInfo m_fileinfo;
    Subtitle m_subtitle;
    Messenger m_applicationMessenger;
    Window m_window;
};

} // namespace DialogueFromVideo
