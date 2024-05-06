#pragma once

#include <QObject>
#include <QTest>

namespace DialogueTest {

class ApplicationTest : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationTest(QObject* parent = nullptr);

signals:

private slots:
    void initTestCase();
    void initTestCase_data();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testRun();
    void testRun_data();

private:
};

} // namespace DialogueTest
