#include <progressbartest.hpp>

#include <common/progressbar.hpp>

using namespace DialogueFromVideo;

namespace DialogueTest {

ProgressBarTest::ProgressBarTest(QObject* parent)
    : QObject(parent)
{
}

void ProgressBarTest::initTestCase()
{
    m_testProgressBar = ProgressBar::instance().progressBar();
}

void ProgressBarTest::initTestCase_data()
{
    QTest::addColumn<int>("initial_value");
    QTest::addColumn<int>("initial_max");

    QTest::newRow("initial_empty_hundred") << 0 << 100;
    QTest::newRow("initial_halfway_hundred") << 50 << 100;
    QTest::newRow("initial_complete_hundred") << 100 << 100;
    QTest::newRow("initial_random_thousand") << 727 << 1000;
}

void ProgressBarTest::cleanupTestCase()
{
    m_testProgressBar = nullptr;
}

void ProgressBarTest::init()
{
    QFETCH_GLOBAL(int, initial_value);
    QFETCH_GLOBAL(int, initial_max);

    m_testProgressBar->reset();
    m_testProgressBar->setMaximum(initial_max);
    m_testProgressBar->setValue(initial_value);
}

void ProgressBarTest::cleanup()
{
}


void ProgressBarTest::testProgressAdd()
{
    QFETCH_GLOBAL(int, initial_value);
    QFETCH_GLOBAL(int, initial_max);

    QFETCH(int, add_value);

    /* note: this progress bar is the inner Qt class
     * and I just like leaving 'Q' out of variable names */
    QCOMPARE(m_testProgressBar->value(), initial_value);
    QCOMPARE(m_testProgressBar->maximum(), initial_max);

    ProgressBar& testProgressBar = ProgressBar::instance();
    testProgressBar.progressAddHandler(add_value);

    int expected_value;

    // This is what's supposed to happen according to Qt docs
    if (initial_value + add_value > initial_max)
    {
        expected_value = initial_value; // stays unchanged
    }
    else
    {
        expected_value = initial_value + add_value;
    }

    QCOMPARE(m_testProgressBar->value(), expected_value);
}

void ProgressBarTest::testProgressAdd_data()
{
    QTest::addColumn<int>("add_value");

    QTest::newRow("add_zero") << 0;
    QTest::newRow("add_one") << 1;
    QTest::newRow("add_twenty") << 20;
    QTest::newRow("add_fifty") << 50;
    QTest::newRow("add_hundred") << 100;
    QTest::newRow("add_thousand") << 1000;
}


void ProgressBarTest::testProgressMaximum()
{
    QFETCH_GLOBAL(int, initial_max);

    QFETCH(int, max_value);

    QCOMPARE(m_testProgressBar->maximum(), initial_max);

    ProgressBar& testProgressBar = ProgressBar::instance();
    testProgressBar.progressMaximumHandler(max_value);

    QCOMPARE(m_testProgressBar->maximum(), max_value);
}

void ProgressBarTest::testProgressMaximum_data()
{
    QTest::addColumn<int>("max_value");

    QTest::newRow("max_zero") << 0;
    QTest::newRow("max_one") << 1;
    QTest::newRow("max_twenty") << 20;
    QTest::newRow("max_fifty") << 50;
    QTest::newRow("max_hundred") << 100;
    QTest::newRow("max_thousand") << 1000;
}


void ProgressBarTest::testProgressComplete()
{
    QFETCH_GLOBAL(int, initial_value);
    QFETCH_GLOBAL(int, initial_max);

    QCOMPARE(m_testProgressBar->value(), initial_value);
    QCOMPARE(m_testProgressBar->maximum(), initial_max);

    ProgressBar& testProgressBar = ProgressBar::instance();
    testProgressBar.progressCompleteHandler();

    QCOMPARE(m_testProgressBar->value(), m_testProgressBar->maximum());
}

void ProgressBarTest::testProgressComplete_data()
{
}


void ProgressBarTest::testProgressReset()
{
    QFETCH_GLOBAL(int, initial_value);
    QFETCH_GLOBAL(int, initial_max);

    QCOMPARE(m_testProgressBar->value(), initial_value);
    QCOMPARE(m_testProgressBar->maximum(), initial_max);

    ProgressBar& testProgressBar = ProgressBar::instance();
    testProgressBar.progressResetHandler();

    // Qt's own reset() function sets the value to minimum - 1 for some reason
    QCOMPARE(m_testProgressBar->value(), m_testProgressBar->minimum() - 1);
}

void ProgressBarTest::testProgressReset_data()
{
}

} // namespace DialogueTest
