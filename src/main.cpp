#include <common/exitcode.hpp>
#include <application.hpp>

#include <QLocale>
#include <QTranslator>
#include <QProcess>
#include <QResource>
#include <QApplication>
#include <QIcon>

using namespace DialogueFromVideo;

int main(int argc, char** argv)
{
    int exitCode = 0;

    QApplication a(argc, argv);

    QGuiApplication::setApplicationName("Video to Dialogue");
    QGuiApplication::setApplicationDisplayName("V₂D");
    QGuiApplication::setApplicationVersion("1.0.4");

    QGuiApplication::setWindowIcon(QIcon(":/icon.ico"));

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
