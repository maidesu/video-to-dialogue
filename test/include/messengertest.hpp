#pragma once

#include <QObject>
#include <QTest>

#include <common/messenger.hpp>

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
};

} // namespace DialogueTest
