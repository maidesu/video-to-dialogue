#include "../include/common/exitcode.hpp"
#include "../include/application.hpp"

#include <QLocale>
#include <QTranslator>
#include <QProcess>
#include <QResource>
#include <QApplication>

using namespace DialogueFromVideo;

int main(int argc, char** argv)
{
    int exitCode = 0;

    QApplication a(argc, argv);

    QGuiApplication::setApplicationName("Video to Dialogue");
    QGuiApplication::setApplicationDisplayName("V2D");
    QGuiApplication::setApplicationVersion("0.0.1");

    Application app;
    app.run();

    exitCode = a.exec();

    if (exitCode == ExitCode::EXIT_CODE_REBOOT)
    {
        QString exe = argv[0];
        QStringList args;

        for (int i = 1; i < argc; ++i)
        {
            args << argv[i];
        }

        QProcess::startDetached(exe, args);
    }

    return exitCode;
}
