#pragma once

#include <filemanager.hpp>
#include <settings.hpp>
#include <subtitle/subtitle.hpp>
#include <widgets/window.hpp>

namespace DialogueFromVideo {

class Application
{
public:
    explicit Application();

    ~Application() = default;

    Application(const Application&) = delete;
    Application(const Application&&) = delete;

    Application& operator= (const Application&) = delete;
    Application& operator= (const Application&&) = delete;

    void run();

private:
    Settings m_settings;
    FileManager m_fileManager;
    Subtitle m_subtitle;
    Messenger m_applicationMessenger;
    Window m_window;
};

} // namespace DialogueFromVideo
