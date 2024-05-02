#pragma once

#include <QObject>
#include <QTest>

#include <common/console.hpp>

namespace DialogueTest {

class ConsoleTest : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleTest(QObject* parent = nullptr);

signals:

private slots:
    void initTestCase();
    void initTestCase_data();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testPrint();
    void testPrint_data();

    void testFilter();
    void testFilter_data();

    void testClear();
    void testClear_data();

private:
    QTextEdit* m_consoleTextEdit;
};

} // namespace DialogueTest
