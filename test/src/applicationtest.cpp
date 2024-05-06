#include <applicationtest.hpp>

#include <application.hpp>

using namespace DialogueFromVideo;

namespace DialogueTest {

ApplicationTest::ApplicationTest(QObject* parent)
    : QObject(parent)
{
}

void ApplicationTest::initTestCase()
{
}

void ApplicationTest::initTestCase_data()
{
}

void ApplicationTest::cleanupTestCase()
{
}

void ApplicationTest::init()
{
}

void ApplicationTest::cleanup()
{
}


void ApplicationTest::testRun()
{
    Application testApp;

    QVERIFY(testApp.run()); /* this is the sole public api provided by Application */
}

void ApplicationTest::testRun_data()
{
}

} // namespace DialogueTest
