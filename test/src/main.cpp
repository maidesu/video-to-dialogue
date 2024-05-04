#include <QCoreApplication>
#include <QtTest>

#include <consoletest.hpp>
#include <formatoptstest.hpp>
#include <messengertest.hpp>

using namespace DialogueTest;

int main(int argc, char** argv)
{
    QApplication a(argc, argv);

    int ret = 0;
    QVector<QObject*> tests;

    tests.append(new ConsoleTest());
    tests.append(new FormatOptionsTest());
    tests.append(new MessengerTest());

    for (QObject* test : tests)
        ret |= QTest::qExec(test, argc, argv);

    return ret;
}
