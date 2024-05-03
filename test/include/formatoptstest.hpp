#pragma once

#include <QObject>
#include <QTest>

#include <common/formatopts.hpp>

namespace DialogueTest {

class FormatOptionsTest : public QObject
{
    Q_OBJECT
public:
    explicit FormatOptionsTest(QObject* parent = nullptr);

signals:

private slots:
    void initTestCase();
    void initTestCase_data();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testLossyOptions();
    void testLossyOptions_data();

    void testLosslessOptions();
    void testLosslessOptions_data();

private:
};

} // namespace DialogueTest
