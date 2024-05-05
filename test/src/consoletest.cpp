#include <consoletest.hpp>

#include <common/console.hpp>

using namespace DialogueFromVideo;

namespace DialogueTest {

ConsoleTest::ConsoleTest(QObject* parent)
    : QObject(parent)
{
}

void ConsoleTest::initTestCase()
{
    m_consoleTextEdit = Console::instance().textEdit();

    QCOMPARE(m_consoleTextEdit->toPlainText(), "");
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
    Console::instance().clearHandler();
}


void ConsoleTest::testPrint()
{
    QFETCH(QString, message);
    QFETCH(QString, id);
    QFETCH(MessageLevel, level);
    QFETCH(QString, print);

    Console::instance().printHandler(message, id, level);

    QCOMPARE(m_consoleTextEdit->toPlainText(), print);
}

void ConsoleTest::testPrint_data()
{
    QTest::addColumn<QString>("message");
    QTest::addColumn<QString>("id");
    QTest::addColumn<MessageLevel>("level");
    QTest::addColumn<QString>("print");

    QTest::newRow("empty") << ""
                           << ""
                           << MessageLevel::Info
                           << "<Info >: \n";

    QTest::newRow("debug") << "My Debug Message."
                           << "ConsoleTester"
                           << MessageLevel::Debug
                           << "<Debug ConsoleTester>: My Debug Message.\n";

    QTest::newRow("info") << "Informative message."
                          << "ConsoleTester"
                          << MessageLevel::Info
                          << "<Info ConsoleTester>: Informative message.\n";

    QTest::newRow("warning") << "Warning message!"
                             << "ConsoleTester"
                             << MessageLevel::Warning
                             << "<Warning ConsoleTester>: Warning message!\n";

    QTest::newRow("error") << "Error message!"
                           << "ConsoleTester"
                           << MessageLevel::Error
                           << "<Error ConsoleTester>: Error message!\n";
}


void ConsoleTest::testFilter()
{
    /* The blockCount of an empty document is 1,
     * therefore n lines on the console will
     * have a block count of n+1 */

    Console::instance().printHandler("Message1",
                                     "id1",
                                     MessageLevel::Info);
    Console::instance().printHandler("Message2",
                                     "id2",
                                     MessageLevel::Warning);
    Console::instance().printHandler("Message3",
                                     "id3",
                                     MessageLevel::Error);
    Console::instance().printHandler("Message2",
                                     "id2",
                                     MessageLevel::Warning);

    QVERIFY( !(m_consoleTextEdit->toPlainText().isEmpty()) ); // Not empty after printing

    Console::instance().filterHandler(MessageLevel::Debug);

    QCOMPARE(m_consoleTextEdit->document()->blockCount(), 4+1); // Expect 4 messages to be filtered

    Console::instance().filterHandler(MessageLevel::Error);

    QVERIFY( !(m_consoleTextEdit->toPlainText().isEmpty()) );
    QCOMPARE(m_consoleTextEdit->document()->blockCount(), 1+1); // Expect 1 Error to be present

    Console::instance().filterHandler(MessageLevel::Warning);

    QCOMPARE(m_consoleTextEdit->document()->blockCount(), 3+1); // Expect 3 messages to be present
}

void ConsoleTest::testFilter_data()
{
}


void ConsoleTest::testClear()
{
    Console::instance().printHandler("Message1",
                                     "id1",
                                     MessageLevel::Info);
    Console::instance().printHandler("Message2",
                                     "id2",
                                     MessageLevel::Warning);
    Console::instance().printHandler("Message3",
                                     "id3",
                                     MessageLevel::Error);

    QVERIFY( !(m_consoleTextEdit->toPlainText().isEmpty()) ); // Not empty after printing

    Console::instance().clearHandler();

    QVERIFY( m_consoleTextEdit->toPlainText().isEmpty() );    // Empty after clearing
}

void ConsoleTest::testClear_data()
{
}

} // namespace DialogueTest
