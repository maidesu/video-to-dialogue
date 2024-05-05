#pragma once

#include <common/messenger.hpp>

#include <QSignalSpy>
#include <QObject>
#include <QTest>

using namespace DialogueFromVideo;

namespace DialogueTest {

class MessengerTest : public QObject
{
    Q_OBJECT
public:
    explicit MessengerTest(QObject* parent = nullptr);

signals:

private slots:
    void initTestCase();
    void initTestCase_data();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testPrint();
    void testPrint_data();

private:
    Messenger* m_testMessenger;
    QSignalSpy* m_signalSpy;
};

} // namespace DialogueTest
