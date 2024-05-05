#include <QCoreApplication>
#include <QtTest>

#include <consoletest.hpp>
#include <formatoptstest.hpp>
#include <messengertest.hpp>
#include <singletontest.hpp>
#include <timetest.hpp>
#include <progresstest.hpp>
#include <progressbartest.hpp>

using namespace DialogueTest;

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    int ret = 0;
    QVector<QObject*> tests;

    // Common modules
    tests.append(new ConsoleTest());
    tests.append(new FormatOptionsTest());
    tests.append(new MessengerTest());
    tests.append(new SingletonTest());
    tests.append(new TimeTest());
    tests.append(new ProgressTest());
    tests.append(new ProgressBarTest());

    for (QObject* test : tests)
        ret |= QTest::qExec(test, argc, argv);

    return ret;
}
