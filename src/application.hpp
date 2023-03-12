#pragma once

#include "common/console.hpp"
#include "widgets/window.hpp"
#include "fileinfo.hpp"
//#include "settings.hpp"

namespace DialogueFromVideo {

class Application
{
public:
    Application();
    ~Application() = default;

    void run();

private:
    Window m_window;
    Console& m_console;
    FileInfo& m_fileinfo;
    //Settings& m_settings;
};

} // namespace DialogueFromVideo
