#include <formatoptstest.hpp>

namespace DialogueTest {

FormatOptionsTest::FormatOptionsTest(QObject* parent)
    : QObject(parent)
{
}

void FormatOptionsTest::initTestCase()
{
}

void FormatOptionsTest::initTestCase_data()
{
}

void FormatOptionsTest::cleanupTestCase()
{
}

void FormatOptionsTest::init()
{
}

void FormatOptionsTest::cleanup()
{
}


void FormatOptionsTest::testLossyOptions()
{
    QFETCH(QString, format);
    QFETCH(int, optionCount);

    QMap<QString, QList<DialogueFromVideo::FormatOptions::Option>> lossy_map = DialogueFromVideo::FormatOptions::lossyOptions();

    QVERIFY(lossy_map.contains(format));
    QCOMPARE(lossy_map[format].size(), optionCount);
}

void FormatOptionsTest::testLossyOptions_data()
{
    QTest::addColumn<QString>("format");
    QTest::addColumn<int>("optionCount");

    QTest::newRow("mp3") << "mp3" << 5;
    QTest::newRow("aac") << "aac" << 13;
    QTest::newRow("opus") << "opus" << 1;
}


void FormatOptionsTest::testLosslessOptions()
{
    QFETCH(QString, format);
    QFETCH(int, optionCount);

    QMap<QString, QList<DialogueFromVideo::FormatOptions::Option>> lossless_map = DialogueFromVideo::FormatOptions::losslessOptions();

    QVERIFY(lossless_map.contains(format));
    QCOMPARE(lossless_map[format].size(), optionCount);
}

void FormatOptionsTest::testLosslessOptions_data()
{
    QTest::addColumn<QString>("format");
    QTest::addColumn<int>("optionCount");

    QTest::newRow("flac") << "flac" << 9;
    QTest::newRow("wav") << "wav" << 3;
}

} // namespace DialogueTest
