#include <QCoreApplication>
#include <QtTest>

#include <consoletest.hpp>

using namespace DialogueTest;

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    int ret = 0;
    QVector<QObject*> tests;

    tests.append(new ConsoleTest);

    for (QObject* test : tests)
        ret |= QTest::qExec(test, argc, argv);

    return ret;
}
