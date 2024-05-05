#include <progresstest.hpp>

#include <QSignalSpy>

namespace DialogueTest {

ProgressTest::ProgressTest(QObject* parent)
    : QObject(parent)
{
}

void ProgressTest::initTestCase()
{
    m_testProgress = new Progress{};
}

void ProgressTest::initTestCase_data()
{
}

void ProgressTest::cleanupTestCase()
{
    delete m_testProgress;
}

void ProgressTest::init()
{
}

void ProgressTest::cleanup()
{
}


void ProgressTest::testProgressAdd()
{
    QSignalSpy signalSpy { m_testProgress, SIGNAL(progressAdd(int)) };

    QCOMPARE(signalSpy.count(), 0);

    emit m_testProgress->progressAdd(1);

    QCOMPARE(signalSpy.count(), 1);

    emit m_testProgress->progressAdd(5);

    QCOMPARE(signalSpy.count(), 2);

    QList<QVariant> args = signalSpy.takeFirst();
    QCOMPARE(args.at(0).toInt(), 1);

    args = signalSpy.takeFirst();
    QCOMPARE(args.at(0).toInt(), 5);
}

void ProgressTest::testProgressAdd_data()
{
}


void ProgressTest::testProgressMaximum()
{
    QSignalSpy signalSpy { m_testProgress, SIGNAL(progressMaximum(int)) };

    QCOMPARE(signalSpy.count(), 0);

    emit m_testProgress->progressMaximum(1);

    QCOMPARE(signalSpy.count(), 1);

    emit m_testProgress->progressMaximum(100);

    QCOMPARE(signalSpy.count(), 2);

    QList<QVariant> args = signalSpy.takeFirst();
    QCOMPARE(args.at(0).toInt(), 1);

    args = signalSpy.takeFirst();
    QCOMPARE(args.at(0).toInt(), 100);
}

void ProgressTest::testProgressMaximum_data()
{
}


void ProgressTest::testProgressComplete()
{
    QSignalSpy signalSpy { m_testProgress, SIGNAL(progressComplete()) };

    emit m_testProgress->progressComplete();

    QCOMPARE(signalSpy.count(), 1);
}

void ProgressTest::testProgressComplete_data()
{
}


void ProgressTest::testProgressReset()
{
    QSignalSpy signalSpy { m_testProgress, SIGNAL(progressReset()) };

    emit m_testProgress->progressReset();

    QCOMPARE(signalSpy.count(), 1);
}

void ProgressTest::testProgressReset_data()
{
}

} // namespace DialogueTest
