#include <messengertest.hpp>

namespace DialogueTest {

MessengerTest::MessengerTest(QObject* parent)
    : QObject(parent)
{
}

void MessengerTest::initTestCase()
{
    m_testMessenger = new Messenger{};
    m_signalSpy = new QSignalSpy{ m_testMessenger, SIGNAL(print(const QString&,
                                                                const QString&,
                                                                const DialogueFromVideo::MessageLevel&)) };
}

void MessengerTest::initTestCase_data()
{
}

void MessengerTest::cleanupTestCase()
{
    delete m_testMessenger;
    delete m_signalSpy;
}

void MessengerTest::init()
{
}

void MessengerTest::cleanup()
{
}


void MessengerTest::testPrint()
{
    QFETCH(QString, message);
    QFETCH(QString, id);
    QFETCH(MessageLevel, level);

    // Trigger print signal
    emit m_testMessenger->print(message,
                                id,
                                level);

    QCOMPARE(m_signalSpy->count(), 1);

    QList<QVariant> args = m_signalSpy->takeFirst();

    QVERIFY(args.at(0).toString() == message);
    QVERIFY(args.at(1).toString() == id);
    QVERIFY(args.at(2).toInt() == static_cast<int>(level));
}

void MessengerTest::testPrint_data()
{
    QTest::addColumn<QString>("message");
    QTest::addColumn<QString>("id");
    QTest::addColumn<MessageLevel>("level");
    QTest::addColumn<int>("position");

    QTest::newRow("msg1") << "Signal 1 sent"
                          << "Messenger"
                          << MessageLevel::Info
                          << 1;

    QTest::newRow("msg2") << "Signal 2 sent"
                          << "Messenger"
                          << MessageLevel::Error
                          << 2;
}

} // namespace DialogueTest
