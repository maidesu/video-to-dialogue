#pragma once

#include <QObject>
#include <QTest>

namespace DialogueTest {

class TimeTest : public QObject
{
    Q_OBJECT
public:
    explicit TimeTest(QObject* parent = nullptr);

signals:

private slots:
    void initTestCase();
    void initTestCase_data();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testMsToStr();
    void testMsToStr_data();

    void testStrToMs();
    void testStrToMs_data();

private:
};

} // namespace DialogueTest
