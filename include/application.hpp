#pragma once

#include <filemanager.hpp>
#include <settings.hpp>
#include <audio/audio.hpp>
#include <subtitle/subtitle.hpp>
#include <frame/frame.hpp>
#include <dialogue/dialogue.hpp>
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

    bool run();

private:
    Settings m_settings;

    Audio m_audio;
    Subtitle m_subtitle;
    Dialogue m_dialogue;
    Frame m_frame;

    FileManager m_fileManager;

    Window m_window;


    Messenger m_applicationMessenger;
};

} // namespace DialogueFromVideo
