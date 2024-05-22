#include <dialoguetest.hpp>

namespace DialogueTest {

ProcessDialogueTest::ProcessDialogueTest(QObject* parent)
    : QObject(parent)
{
}

void ProcessDialogueTest::initTestCase()
{
    m_testDialogue = new Dialogue{};

    m_signalSpy = new QSignalSpy{ m_testDialogue, SIGNAL(readyDialogueSignal(const QList<Interval>&,
                                                                             const QList<Interval>&,
                                                                             const QList<Interval>&,
                                                                             const QList<Interval>&)) };

    m_testSubs = new QList<SubEntry*>
    {
        new SubEntry{ 0, 1000, "This subtitle lasted for 1 second." },
        new SubEntry{ 1000, 1500, "There was no gap, yet here we are in a new sub!" },
        new SubEntry{ 2100, 5000, "There was a slight pause, that's also intentionally small." },
        new SubEntry{ 10000, 12000, "After a 5 second pause, I finally typed again." },
        new SubEntry{ 13000, 33000, "After a brief pause, I slowly typed for a whole twenty seconds." },
        new SubEntry{ 33000, 33000, "" },
        new SubEntry{ 35000, 38000, "I tried talking again, but the words won't come out." },
        new SubEntry{ 38100, 38200, "P" }, // 100d + 100p - 100 dialogue 100 padding
        new SubEntry{ 38300, 38400, "a" },
        new SubEntry{ 38500, 38600, "d" },
        new SubEntry{ 38800, 38900, "d" }, // 100d + 200p
        new SubEntry{ 39100, 39200, "i" },
        new SubEntry{ 39400, 39500, "i" },
        new SubEntry{ 40000, 40100, "n" },  // 100d + 500p
        new SubEntry{ 40600, 40700, "g" },
        new SubEntry{ 41200, 41300, "." }
    };
}

void ProcessDialogueTest::initTestCase_data()
{
}

void ProcessDialogueTest::cleanupTestCase()
{
    delete m_signalSpy;
    delete m_testDialogue;

    for (SubEntry* se : *m_testSubs)
    {
        delete se;
    }

    m_testSubs->clear();
    delete m_testSubs;
}

void ProcessDialogueTest::init()
{
}

void ProcessDialogueTest::cleanup()
{
}


void ProcessDialogueTest::testProcessDialogue()
{
    QFETCH(qint64, left_margin);
    QFETCH(qint64, right_margin);
    QFETCH(qint64, offset);
    QFETCH(qint64, minimum_gap);

    QFETCH(QList<Interval>, expected_dialogue);
    QFETCH(QList<Interval>, expected_subtitle);
    QFETCH(QList<Interval>, expected_padding);
    QFETCH(QList<Interval>, expected_gap);

    m_testDialogue->processDialogueHandler(*m_testSubs,
                                           left_margin,
                                           right_margin,
                                           offset,
                                           minimum_gap);

    //m_signalSpy->wait(); - don't need this apparently
    QCOMPARE(m_signalSpy->count(), 1);

    QList<QVariant> args = m_signalSpy->takeFirst();

    const QList<Interval>& actual_dialogue = args.at(0).value<QList<Interval>>();
    const QList<Interval>& actual_subtitle = args.at(1).value<QList<Interval>>();
    const QList<Interval>& actual_padding  = args.at(2).value<QList<Interval>>();
    const QList<Interval>& actual_gap      = args.at(3).value<QList<Interval>>();

    QCOMPARE(actual_dialogue.size(), expected_dialogue.size());
    QCOMPARE(actual_subtitle.size(), expected_subtitle.size());
    QCOMPARE(actual_padding.size(), expected_padding.size());
    QCOMPARE(actual_gap.size(), expected_gap.size());

    for (qsizetype i = 0; i < expected_dialogue.size(); ++i)
    {
        QCOMPARE(actual_dialogue.at(i), expected_dialogue.at(i));
    }
    for (qsizetype i = 0; i < expected_subtitle.size(); ++i)
    {
        QCOMPARE(actual_subtitle.at(i), expected_subtitle.at(i));
    }
    for (qsizetype i = 0; i < expected_padding.size(); ++i)
    {
        QCOMPARE(actual_padding.at(i), expected_padding.at(i));
    }
    for (qsizetype i = 0; i < expected_gap.size(); ++i)
    {
        QCOMPARE(actual_gap.at(i), expected_gap.at(i));
    }
}

void ProcessDialogueTest::testProcessDialogue_data()
{
    QTest::addColumn<qint64>("left_margin");
    QTest::addColumn<qint64>("right_margin");
    QTest::addColumn<qint64>("offset");
    QTest::addColumn<qint64>("minimum_gap");

    QTest::addColumn<QList<Interval>>("expected_dialogue");
    QTest::addColumn<QList<Interval>>("expected_subtitle");
    QTest::addColumn<QList<Interval>>("expected_padding");
    QTest::addColumn<QList<Interval>>("expected_gap");


    QTest::newRow("no_margin_no_offset_no_gap") << 0LL << 0LL << 0LL << 0LL
                                                << QList<Interval> // 0 length Interval should be missing
                                                   {
                                                       { 0, 1000 },
                                                       { 1000, 1500 },
                                                       { 2100, 5000 },
                                                       { 10000, 12000 },
                                                       { 13000, 33000 },
                                                       { 35000, 38000 },
                                                       { 38100, 38200 },
                                                       { 38300, 38400 },
                                                       { 38500, 38600 },
                                                       { 38800, 38900 },
                                                       { 39100, 39200 },
                                                       { 39400, 39500 },
                                                       { 40000, 40100 },
                                                       { 40600, 40700 },
                                                       { 41200, 41300 }
                                                   }
                                                << QList<Interval>
                                                   {
                                                       { 0, 1000 },
                                                       { 1000, 1500 },
                                                       { 2100, 5000 },
                                                       { 10000, 12000 },
                                                       { 13000, 33000 },
                                                       { 33000, 33000 }, // 0 len is included here of course
                                                       { 35000, 38000 },
                                                       { 38100, 38200 },
                                                       { 38300, 38400 },
                                                       { 38500, 38600 },
                                                       { 38800, 38900 },
                                                       { 39100, 39200 },
                                                       { 39400, 39500 },
                                                       { 40000, 40100 },
                                                       { 40600, 40700 },
                                                       { 41200, 41300 }
                                                   }
                                                << QList<Interval>
                                                   {
                                                       { 0, 0 },            { 1000, 1000 },
                                                       { 1000, 1000 },      { 1500, 1500 },
                                                       { 2100, 2100 },      { 5000, 5000 },
                                                       { 10000, 10000 },    { 12000, 12000 },
                                                       { 13000, 13000 },    { 33000, 33000 },
                                                       { 33000, 33000 },    { 33000, 33000 },
                                                       { 35000, 35000 },    { 38000, 38000 },
                                                       { 38100, 38100 },    { 38200, 38200 },
                                                       { 38300, 38300 },    { 38400, 38400 },
                                                       { 38500, 38500 },    { 38600, 38600 },
                                                       { 38800, 38800 },    { 38900, 38900 },
                                                       { 39100, 39100 },    { 39200, 39200 },
                                                       { 39400, 39400 },    { 39500, 39500 },
                                                       { 40000, 40000 },    { 40100, 40100 },
                                                       { 40600, 40600 },    { 40700, 40700 },
                                                       { 41200, 41200 },    { 41300, 41300 }
                                                   }
                                                << QList<Interval>{}; // empty

    QTest::newRow("no_margin_backward_offset_no_gap") << 0LL << 0LL << -1000LL << 0LL
                                                << QList<Interval>
                                                   {
                                                       { -1000, 0 },
                                                       { 0, 500 },
                                                       { 1100, 4000 },
                                                       { 9000, 11000 },
                                                       { 12000, 32000 },
                                                       { 34000, 37000 },
                                                       { 37100, 37200 },
                                                       { 37300, 37400 },
                                                       { 37500, 37600 },
                                                       { 37800, 37900 },
                                                       { 38100, 38200 },
                                                       { 38400, 38500 },
                                                       { 39000, 39100 },
                                                       { 39600, 39700 },
                                                       { 40200, 40300 }
                                                   }
                                                << QList<Interval>
                                                   {
                                                       { -1000, 0 },
                                                       { 0, 500 },
                                                       { 1100, 4000 },
                                                       { 9000, 11000 },
                                                       { 12000, 32000 },
                                                       { 32000, 32000 },
                                                       { 34000, 37000 },
                                                       { 37100, 37200 },
                                                       { 37300, 37400 },
                                                       { 37500, 37600 },
                                                       { 37800, 37900 },
                                                       { 38100, 38200 },
                                                       { 38400, 38500 },
                                                       { 39000, 39100 },
                                                       { 39600, 39700 },
                                                       { 40200, 40300 }
                                                   }
                                                << QList<Interval>
                                                   {
                                                       { -1000, -1000 },    { 0, 0 },
                                                       { 0, 0 },            { 500, 500 },
                                                       { 1100, 1100 },      { 4000, 4000 },
                                                       { 9000, 9000 },      { 11000, 11000 },
                                                       { 12000, 12000 },    { 32000, 32000 },
                                                       { 32000, 32000 },    { 32000, 32000 },
                                                       { 34000, 34000 },    { 37000, 37000 },
                                                       { 37100, 37100 },    { 37200, 37200 },
                                                       { 37300, 37300 },    { 37400, 37400 },
                                                       { 37500, 37500 },    { 37600, 37600 },
                                                       { 37800, 37800 },    { 37900, 37900 },
                                                       { 38100, 38100 },    { 38200, 38200 },
                                                       { 38400, 38400 },    { 38500, 38500 },
                                                       { 39000, 39000 },    { 39100, 39100 },
                                                       { 39600, 39600 },    { 39700, 39700 },
                                                       { 40200, 40200 },    { 40300, 40300 }
                                                   }
                                                << QList<Interval>{}; // empty

    QTest::newRow("standard_margin_no_offset_no_gap") << 200LL << 200LL << 0LL << 0LL
                                                      << QList<Interval> // expected result calculated manually
                                                         {
                                                             { -200, 1700 },
                                                             { 1900, 5200 },
                                                             { 9800, 12200 },
                                                             { 12800, 33200 },
                                                             { 34800, 39700 },
                                                             { 39800, 40300 },
                                                             { 40400, 40900 },
                                                             { 41000, 41500 }
                                                         }
                                                      << QList<Interval>
                                                         {
                                                             { 0, 1000 },
                                                             { 1000, 1500 },
                                                             { 2100, 5000 },
                                                             { 10000, 12000 },
                                                             { 13000, 33000 },
                                                             { 33000, 33000 },
                                                             { 35000, 38000 },
                                                             { 38100, 38200 },
                                                             { 38300, 38400 },
                                                             { 38500, 38600 },
                                                             { 38800, 38900 },
                                                             { 39100, 39200 },
                                                             { 39400, 39500 },
                                                             { 40000, 40100 },
                                                             { 40600, 40700 },
                                                             { 41200, 41300 }
                                                         }
                                                      << QList<Interval>
                                                         {
                                                             { -200, 0 },         { 1000, 1200 },
                                                             { 800, 1000 },       { 1500, 1700 },
                                                             { 1900, 2100 },      { 5000, 5200 },
                                                             { 9800, 10000 },     { 12000, 12200 },
                                                             { 12800, 13000 },    { 33000, 33200 },
                                                             { 32800, 33000 },    { 33000, 33200 },
                                                             { 34800, 35000 },    { 38000, 38200 },
                                                             { 37900, 38100 },    { 38200, 38400 },
                                                             { 38100, 38300 },    { 38400, 38600 },
                                                             { 38300, 38500 },    { 38600, 38800 },
                                                             { 38600, 38800 },    { 38900, 39100 },
                                                             { 38900, 39100 },    { 39200, 39400 },
                                                             { 39200, 39400 },    { 39500, 39700 },
                                                             { 39800, 40000 },    { 40100, 40300 },
                                                             { 40400, 40600 },    { 40700, 40900 },
                                                             { 41000, 41200 },    { 41300, 41500 }
                                                         }
                                                      << QList<Interval>{};

    QTest::newRow("negative_margin_no_offset_no_gap") << -200LL << -200LL << 0LL << 0LL
                                                      << QList<Interval>
                                                         {
                                                             { 200, 800 },
                                                             { 1200, 1300 },
                                                             { 2300, 4800 },
                                                             { 10200, 11800 },
                                                             { 13200, 32800 },
                                                             { 35200, 37800 }
                                                         }
                                                      << QList<Interval>
                                                         {
                                                             { 0, 1000 },
                                                             { 1000, 1500 },
                                                             { 2100, 5000 },
                                                             { 10000, 12000 },
                                                             { 13000, 33000 },
                                                             { 33000, 33000 },
                                                             { 35000, 38000 },
                                                             { 38100, 38200 },
                                                             { 38300, 38400 },
                                                             { 38500, 38600 },
                                                             { 38800, 38900 },
                                                             { 39100, 39200 },
                                                             { 39400, 39500 },
                                                             { 40000, 40100 },
                                                             { 40600, 40700 },
                                                             { 41200, 41300 }
                                                         }
                                                      << QList<Interval>
                                                         /* We need to be able to represent
                                                          * negative margin, start >= end is allowed */
                                                         {
                                                             { 200, 0 },          { 1000, 800 },
                                                             { 1200, 1000 },      { 1500, 1300 },
                                                             { 2300, 2100 },      { 5000, 4800 },
                                                             { 10200, 10000 },    { 12000, 11800 },
                                                             { 13200, 13000 },    { 33000, 32800 },
                                                             { 33200, 33000 },    { 33000, 32800 },
                                                             { 35200, 35000 },    { 38000, 37800 },
                                                             { 38300, 38100 },    { 38200, 38000 },
                                                             { 38500, 38300 },    { 38400, 38200 },
                                                             { 38700, 38500 },    { 38600, 38400 },
                                                             { 39000, 38800 },    { 38900, 38700 },
                                                             { 39300, 39100 },    { 39200, 39000 },
                                                             { 39600, 39400 },    { 39500, 39300 },
                                                             { 40200, 40000 },    { 40100, 39900 },
                                                             { 40800, 40600 },    { 40700, 40500 },
                                                             { 41400, 41200 },    { 41300, 41100 }
                                                         }
                                                      << QList<Interval>{};

    /* Only >=0 gap is allowed, negative gaps make no sense under any circumstance,
     * unlike negative offsets or negative paddings */
    QTest::newRow("no_margin_no_offset_standard_gap") << 0LL << 0LL << 0LL << 1000LL
                                                      << QList<Interval>
                                                         {
                                                             { 0, 5000 },
                                                             { 10000, 12000 },
                                                             { 13000, 33000 },
                                                             { 35000, 41300 }
                                                         }
                                                      << QList<Interval>
                                                         /* only affected by offset,
                                                          * stays the same, of course */
                                                         {
                                                             { 0, 1000 },
                                                             { 1000, 1500 },
                                                             { 2100, 5000 },
                                                             { 10000, 12000 },
                                                             { 13000, 33000 },
                                                             { 33000, 33000 },
                                                             { 35000, 38000 },
                                                             { 38100, 38200 },
                                                             { 38300, 38400 },
                                                             { 38500, 38600 },
                                                             { 38800, 38900 },
                                                             { 39100, 39200 },
                                                             { 39400, 39500 },
                                                             { 40000, 40100 },
                                                             { 40600, 40700 },
                                                             { 41200, 41300 }
                                                         }
                                                      << QList<Interval>
                                                         /* padding unaffected */
                                                         {
                                                             { 0, 0 },            { 1000, 1000 },
                                                             { 1000, 1000 },      { 1500, 1500 },
                                                             { 2100, 2100 },      { 5000, 5000 },
                                                             { 10000, 10000 },    { 12000, 12000 },
                                                             { 13000, 13000 },    { 33000, 33000 },
                                                             { 33000, 33000 },    { 33000, 33000 },
                                                             { 35000, 35000 },    { 38000, 38000 },
                                                             { 38100, 38100 },    { 38200, 38200 },
                                                             { 38300, 38300 },    { 38400, 38400 },
                                                             { 38500, 38500 },    { 38600, 38600 },
                                                             { 38800, 38800 },    { 38900, 38900 },
                                                             { 39100, 39100 },    { 39200, 39200 },
                                                             { 39400, 39400 },    { 39500, 39500 },
                                                             { 40000, 40000 },    { 40100, 40100 },
                                                             { 40600, 40600 },    { 40700, 40700 },
                                                             { 41200, 41200 },    { 41300, 41300 }
                                                         }
                                                      << QList<Interval>
                                                         {
                                                             { 1500, 2100 },
                                                             { 38000, 38100 },
                                                             { 38200, 38300 },
                                                             { 38400, 38500 },
                                                             { 38600, 38800 },
                                                             { 38900, 39100 },
                                                             { 39200, 39400 },
                                                             { 39500, 40000 },
                                                             { 40100, 40600 },
                                                             { 40700, 41200 }
                                                         };
}


void ProcessDialogueTest::testClearDialogue()
{
    QFETCH(qint64, left_margin);
    QFETCH(qint64, right_margin);
    QFETCH(qint64, offset);
    QFETCH(qint64, minimum_gap);

    m_testDialogue->processDialogueHandler(*m_testSubs,
                                           left_margin,
                                           right_margin,
                                           offset,
                                           minimum_gap);

    QCOMPARE(m_signalSpy->count(), 1);

    QList<QVariant> args = m_signalSpy->takeFirst();

    QList<Interval> actual_dialogue = args.at(0).value<QList<Interval>>();
    QList<Interval> actual_subtitle = args.at(1).value<QList<Interval>>();
    QList<Interval> actual_padding  = args.at(2).value<QList<Interval>>();
    QList<Interval> actual_gap      = args.at(3).value<QList<Interval>>();

    QVERIFY(!actual_dialogue.isEmpty());
    QVERIFY(!actual_subtitle.isEmpty());
    QVERIFY(!actual_padding.isEmpty());
    QVERIFY(!actual_gap.isEmpty());

    // Call clear here
    m_testDialogue->clearDialogueHandler();

    // Workaround since QSignalSpy can't pass a reference
    m_testDialogue->processDialogueHandler(QList<SubEntry*>{}, 0, 0, 0, 0);

    QCOMPARE(m_signalSpy->count(), 1);

    args = m_signalSpy->takeFirst();

    actual_dialogue = args.at(0).value<QList<Interval>>();
    actual_subtitle = args.at(1).value<QList<Interval>>();
    actual_padding  = args.at(2).value<QList<Interval>>();
    actual_gap      = args.at(3).value<QList<Interval>>();

    QVERIFY(actual_dialogue.isEmpty());
    QVERIFY(actual_subtitle.isEmpty());
    QVERIFY(actual_padding.isEmpty());
    QVERIFY(actual_gap.isEmpty());
}

void ProcessDialogueTest::testClearDialogue_data()
{
    QTest::addColumn<qint64>("left_margin");
    QTest::addColumn<qint64>("right_margin");
    QTest::addColumn<qint64>("offset");
    QTest::addColumn<qint64>("minimum_gap");

    QTest::newRow("zero_setting") << 0LL << 0LL << 0LL << 1000LL;
    QTest::newRow("randomized_setting") << 543LL << -234LL << -98765LL << 1010LL;
}

} // namespace DialogueTest
