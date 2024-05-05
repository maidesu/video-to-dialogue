#pragma once

#include <common/progress.hpp>

#include <QObject>
#include <QTest>

using namespace DialogueFromVideo;

namespace DialogueTest {

class ProgressTest : public QObject
{
    Q_OBJECT
public:
    explicit ProgressTest(QObject* parent = nullptr);

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
    Progress* m_testProgress;
};

} // namespace DialogueTest
