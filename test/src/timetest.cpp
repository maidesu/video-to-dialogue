#include <timetest.hpp>

#include <common/time.hpp>

using namespace DialogueFromVideo;

namespace DialogueTest {

TimeTest::TimeTest(QObject* parent)
    : QObject(parent)
{
}

void TimeTest::initTestCase()
{
}

void TimeTest::initTestCase_data()
{
}

void TimeTest::cleanupTestCase()
{
}

void TimeTest::init()
{
}

void TimeTest::cleanup()
{
}


void TimeTest::testMsToStr()
{
    QFETCH(int64_t, input_ms);
    QFETCH(QString, expected_str);

    QCOMPARE(Time::millisecondsToStringTime(input_ms), expected_str);
}

void TimeTest::testMsToStr_data()
{
    QTest::addColumn<int64_t>("input_ms");
    QTest::addColumn<QString>("expected_str");

    QTest::newRow("zero") << 0LL << "00:00:00.000";
    QTest::newRow("positive") << 3600000LL << "01:00:00.000";
    QTest::newRow("negative") << -3600000LL << "23:00:00.000";
    QTest::newRow("wrap") << 123456789LL << "10:17:36.789";
}


void TimeTest::testStrToMs()
{
    QFETCH(QString, input_str);
    QFETCH(int64_t, expected_ms);

    QCOMPARE(Time::stringTimeToMilliseconds(input_str), expected_ms);
}

void TimeTest::testStrToMs_data()
{
    QTest::addColumn<QString>("input_str");
    QTest::addColumn<int64_t>("expected_ms");

    QTest::newRow("zero") << "00:00:00.000" << 0LL;
    QTest::newRow("positive") << "12:34:56.789" << 45296789LL;
    QTest::newRow("invalid negative") << "-00:00:01.000" << -1LL;
    QTest::newRow("invalid overflow") << "34:17:36.789" << -1LL;
    QTest::newRow("invalid number") << "12345" << -1LL;
    QTest::newRow("invalid time") << "12::34:56.789" << -1LL;
    QTest::newRow("invalid string") << "twelve hours" << -1LL;
}

} // namespace DialogueTest
