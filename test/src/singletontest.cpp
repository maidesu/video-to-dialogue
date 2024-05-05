#include <singletontest.hpp>

#include <common/singleton.hpp>

using namespace DialogueFromVideo;

namespace DialogueTest {

SingletonTest::SingletonTest(QObject* parent)
    : QObject(parent)
{
}

void SingletonTest::initTestCase()
{
}

void SingletonTest::initTestCase_data()
{
}

void SingletonTest::cleanupTestCase()
{
}

void SingletonTest::init()
{
}

void SingletonTest::cleanup()
{
}


void SingletonTest::testSingleton()
{
    int& instance_a = Singleton<int>::instance();
    int& instance_b = Singleton<int>::instance();
    int& instance_c = Singleton<int>::instance();

    QVERIFY(&instance_a == &instance_b);
    QVERIFY(&instance_b == &instance_c);
}

} // namespace DialogueTest
