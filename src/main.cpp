#include "application.hpp"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

using namespace DialogueFromVideo;

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("Dialogue From Video");

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
