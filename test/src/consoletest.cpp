#include <consoletest.hpp>

namespace DialogueTest {

ConsoleTest::ConsoleTest(QObject* parent)
    : QObject(parent)
{
}

void ConsoleTest::initTestCase()
{
    m_consoleTextEdit = DialogueFromVideo::Console::instance().textEdit();
}

void ConsoleTest::initTestCase_data()
{
}

void ConsoleTest::cleanupTestCase()
{
    m_consoleTextEdit = nullptr;
}

void ConsoleTest::init()
{
}

void ConsoleTest::cleanup()
{
    DialogueFromVideo::Console::instance().clearHandler();
}


void ConsoleTest::testPrint()
{
    QFETCH(QString, message);
    QFETCH(QString, id);
    QFETCH(DialogueFromVideo::MessageLevel, level);
    QFETCH(QString, print);

    DialogueFromVideo::Console::instance().printHandler(message, id, level);

    QCOMPARE(m_consoleTextEdit->toPlainText(), print);
}

void ConsoleTest::testPrint_data()
{
    QTest::addColumn<QString>("message");
    QTest::addColumn<QString>("id");
    QTest::addColumn<DialogueFromVideo::MessageLevel>("level");
    QTest::addColumn<QString>("print");

    QTest::newRow("empty") << ""
                           << ""
                           << DialogueFromVideo::MessageLevel::Info
                           << "<Info >: \n";

    QTest::newRow("debug") << "My Debug Message."
                           << "ConsoleTester"
                           << DialogueFromVideo::MessageLevel::Debug
                           << "<Debug ConsoleTester>: My Debug Message.\n";

    QTest::newRow("info") << "Informative message."
                          << "ConsoleTester"
                          << DialogueFromVideo::MessageLevel::Info
                          << "<Info ConsoleTester>: Informative message.\n";

    QTest::newRow("warning") << "Warning message!"
                             << "ConsoleTester"
                             << DialogueFromVideo::MessageLevel::Warning
                             << "<Warning ConsoleTester>: Warning message!\n";

    QTest::newRow("error") << "Error message!"
                           << "ConsoleTester"
                           << DialogueFromVideo::MessageLevel::Error
                           << "<Error ConsoleTester>: Error message!\n";
}


void ConsoleTest::testFilter()
{

}

void ConsoleTest::testFilter_data()
{

}


void ConsoleTest::testClear()
{

}

void ConsoleTest::testClear_data()
{

}

} // namespace DialogueTest
