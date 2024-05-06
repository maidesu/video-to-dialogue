#pragma once

#include <dialogue/dialogue.hpp>

#include <QSignalSpy>
#include <QObject>
#include <QTest>

using namespace DialogueFromVideo;

namespace DialogueTest {

class ProcessDialogueTest : public QObject
{
    Q_OBJECT
public:
    explicit ProcessDialogueTest(QObject* parent = nullptr);

signals:

private slots:
    void initTestCase();
    void initTestCase_data();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testProcessDialogue();
    void testProcessDialogue_data();

    void testClearDialogue();
    void testClearDialogue_data();

private:
    Dialogue* m_testDialogue;
    QSignalSpy* m_signalSpy;
    QList<SubEntry*>* m_testSubs;
};

} // namespace DialogueTest
