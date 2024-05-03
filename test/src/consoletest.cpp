#include <consoletest.hpp>

namespace DialogueTest {

ConsoleTest::ConsoleTest(QObject* parent)
    : QObject(parent)
{
}

void ConsoleTest::initTestCase()
{
    m_consoleTextEdit = DialogueFromVideo::Console::instance().textEdit();

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
    /* The blockCount of an empty document is 1,
     * therefore n lines on the console will
     * have a block count of n+1 */

    DialogueFromVideo::Console::instance().printHandler("Message1",
                                                        "id1",
                                                        DialogueFromVideo::MessageLevel::Info);
    DialogueFromVideo::Console::instance().printHandler("Message2",
                                                        "id2",
                                                        DialogueFromVideo::MessageLevel::Warning);
    DialogueFromVideo::Console::instance().printHandler("Message3",
                                                        "id3",
                                                        DialogueFromVideo::MessageLevel::Error);
    DialogueFromVideo::Console::instance().printHandler("Message2",
                                                        "id2",
                                                        DialogueFromVideo::MessageLevel::Warning);

    QVERIFY( !(m_consoleTextEdit->toPlainText().isEmpty()) ); // Not empty after printing

    DialogueFromVideo::Console::instance().filterHandler(DialogueFromVideo::MessageLevel::Debug);

    QCOMPARE(m_consoleTextEdit->document()->blockCount(), 4+1); // Expect 4 messages to be filtered

    DialogueFromVideo::Console::instance().filterHandler(DialogueFromVideo::MessageLevel::Error);

    QVERIFY( !(m_consoleTextEdit->toPlainText().isEmpty()) );
    QCOMPARE(m_consoleTextEdit->document()->blockCount(), 1+1); // Expect 1 Error to be present

    DialogueFromVideo::Console::instance().filterHandler(DialogueFromVideo::MessageLevel::Warning);

    QCOMPARE(m_consoleTextEdit->document()->blockCount(), 3+1); // Expect 3 messages to be present
}

void ConsoleTest::testFilter_data()
{
}


void ConsoleTest::testClear()
{
    DialogueFromVideo::Console::instance().printHandler("Message1",
                                                        "id1",
                                                        DialogueFromVideo::MessageLevel::Info);
    DialogueFromVideo::Console::instance().printHandler("Message2",
                                                        "id2",
                                                        DialogueFromVideo::MessageLevel::Warning);
    DialogueFromVideo::Console::instance().printHandler("Message3",
                                                        "id3",
                                                        DialogueFromVideo::MessageLevel::Error);

    QVERIFY( !(m_consoleTextEdit->toPlainText().isEmpty()) ); // Not empty after printing

    DialogueFromVideo::Console::instance().clearHandler();

    QVERIFY( m_consoleTextEdit->toPlainText().isEmpty() );    // Empty after clearing
}

void ConsoleTest::testClear_data()
{
}

} // namespace DialogueTest
