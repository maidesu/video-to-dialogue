#include "../include/application.hpp"

#include <QLocale>
#include <QTranslator>
#include <QResource>
#include <QApplication>

using namespace DialogueFromVideo;

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    QGuiApplication::setApplicationName("Video to Dialogue");
    QGuiApplication::setApplicationDisplayName("V2D");
    QGuiApplication::setApplicationVersion("0.0.1");

    Application app;
    app.run();

    return a.exec();
}
