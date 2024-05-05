#pragma once

#include <QObject>
#include <QTest>

namespace DialogueTest {

class SingletonTest : public QObject
{
    Q_OBJECT
public:
    explicit SingletonTest(QObject* parent = nullptr);

signals:

private slots:
    void initTestCase();
    void initTestCase_data();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testSingleton();

private:
};

} // namespace DialogueTest
