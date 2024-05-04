#include <messengertest.hpp>

#include <QSignalSpy>

namespace DialogueTest {

MessengerTest::MessengerTest(QObject* parent)
    : QObject(parent)
{
}

void MessengerTest::initTestCase()
{
}

void MessengerTest::initTestCase_data()
{
}

void MessengerTest::cleanupTestCase()
{
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
    QFETCH(DialogueFromVideo::MessageLevel, level);

    DialogueFromVideo::Messenger* test_messenger = new DialogueFromVideo::Messenger();

    QSignalSpy spy{test_messenger, SIGNAL(print(const QString&,
                                                const QString&,
                                                const DialogueFromVideo::MessageLevel&))};

    // Trigger print signal
    emit test_messenger->print(message,
                               id,
                               level);

    QCOMPARE(spy.count(), 1);

    QList<QVariant> args = spy.takeFirst();

    QVERIFY(args.at(0).toString() == message);
    QVERIFY(args.at(1).toString() == id);
    QVERIFY(args.at(2).toInt() == static_cast<int>(level));
}

void MessengerTest::testPrint_data()
{
    QTest::addColumn<QString>("message");
    QTest::addColumn<QString>("id");
    QTest::addColumn<DialogueFromVideo::MessageLevel>("level");

    QTest::newRow("msg") << "Signal sent"
                           << "Messenger"
                           << DialogueFromVideo::MessageLevel::Info;
}

} // namespace DialogueTest
