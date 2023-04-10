#include "../include/application.hpp"

#include <QLocale>
#include <QTranslator>
#include <QApplication>

using namespace DialogueFromVideo;

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    QGuiApplication::setApplicationName("Video To Dialogue");
    QGuiApplication::setApplicationDisplayName("V2D");
    QGuiApplication::setApplicationVersion("0.0.1");

    /*QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "dialogue_from_video_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }*/

    Application app;
    app.run();

    return a.exec();
}
