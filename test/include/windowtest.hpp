#pragma once

#include <widgets/window.hpp>
#include <filemanager.hpp>

#include <QObject>
#include <QTest>

using namespace DialogueFromVideo;

namespace DialogueTest {

class TestFileManager : public FileManager
{
    Q_OBJECT
public slots:
    void openFileTestHandler();

public:
    void setTestPath(QString path);
    void clearTestPath();

private:
    bool openFileTest();

    QByteArray* m_testPathBytes;
    const char* m_testPath;

signals:
    void openFileTestOpened();
    void openFileTestRejected();
};

class WindowTest : public QObject
{
    Q_OBJECT
public:
    explicit WindowTest(QObject* parent = nullptr);

signals:

private slots:
    void initTestCase();
    void initTestCase_data();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testOpenFile();
    void testOpenFile_data();

private:
    Window* m_window;
    TestFileManager* m_testFileManager;
};

} // namespace DialogueTest
