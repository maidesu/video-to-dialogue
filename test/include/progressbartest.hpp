#pragma once

#include <QObject>
#include <QTest>
#include <QProgressBar>

namespace DialogueTest {

class ProgressBarTest : public QObject
{
    Q_OBJECT
public:
    explicit ProgressBarTest(QObject* parent = nullptr);

signals:

private slots:
    void initTestCase();
    void initTestCase_data();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testProgressAdd();
    void testProgressAdd_data();

    void testProgressMaximum();
    void testProgressMaximum_data();

    void testProgressComplete();
    void testProgressComplete_data();

    void testProgressReset();
    void testProgressReset_data();

private:
    QProgressBar* m_testProgressBar;
};

} // namespace DialogueTest
