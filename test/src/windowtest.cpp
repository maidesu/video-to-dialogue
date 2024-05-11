#include <windowtest.hpp>

#include <QSignalSpy>
#include <QDir>

#define _STR(x) #x
#define STR(x)  _STR(x)

namespace DialogueTest {

void TestFileManager::openFileTestHandler()
{
    if (openFileTest())
    {
        emit openFileTestOpened();
    }
    else
    {
        emit openFileTestRejected();
    }
}

void TestFileManager::setTestPath(QString path)
{
    QString dir { STR(PROJECT_PATH) + QString{"/data/"} };

    QString platformAbsolute =  QDir::toNativeSeparators(QDir(dir).absoluteFilePath(path));

    m_testPathBytes = new QByteArray{ platformAbsolute.toUtf8().constData() };
#ifdef Q_OS_WIN
    (*m_testPathBytes)[0] = (*m_testPathBytes)[1];
    (*m_testPathBytes)[1] = ':';
#endif
    m_testPath = m_testPathBytes->constData();
}

void TestFileManager::clearTestPath()
{
    delete m_testPathBytes;

    m_testPathBytes = nullptr;
    m_testPath = nullptr;
}

bool TestFileManager::openFileTest()
{
    m_path = m_testPath;

    // Invalidate dialogue export
    m_dialogueList = nullptr;
    m_dialogueFlag = 0;

    return getFileInfo();
}


WindowTest::WindowTest(QObject* parent)
    : QObject(parent)
{
}

void WindowTest::initTestCase()
{
    m_testFileManager = new TestFileManager();
    m_window          = new Window();

    /* All connections between
     * m_fileManager and m_window
     * according to Application.cpp */

    // Ask FileManager to open file from window
    QObject::connect(m_window,
                     &Window::openFileSignal,
                     m_testFileManager,
                     &TestFileManager::openFileTestHandler);

    //// Window requests sub description from FileManager by index
    //QObject::connect(m_window,
    //                 &Window::subDescriptionRequestedSignal,
    //                 m_testFileManager,
    //                 &TestFileManager::subDescriptionRequestedHandler);

    //// Window requests sub layer change from FileManager by index
    //QObject::connect(m_window,
    //                 &Window::subLayerRequestedSignal,
    //                 m_testFileManager,
    //                 &TestFileManager::subLayerRequestedHandler);

    //// Window requests audio description from FileManager by index
    //QObject::connect(m_window,
    //                 &Window::audioDescriptionRequestedSignal,
    //                 m_testFileManager,
    //                 &TestFileManager::audioDescriptionRequestedHandler);

    //// Window requests sampleRate of current audio
    //QObject::connect(m_window,
    //                 &Window::sampleRateRequestedSignal,
    //                 m_testFileManager,
    //                 &TestFileManager::sampleRateRequestedHandler);

    // Window requests file processing from FileManager
    //QObject::connect(m_window,
    //                 &Window::processFileSignal,
    //                 m_testFileManager,
    //                 &TestFileManager::processFileHandler);

    //// Window asks FileManager for a subtitle export
    //QObject::connect(m_window,
    //                 &Window::exportSubtitleSignal,
    //                 m_testFileManager,
    //                 &TestFileManager::exportSubtitleHandler);

    //// Window ask FileManager for a picture collection export
    //QObject::connect(m_window,
    //                 &Window::exportPictureCollectionSignal,
    //                 m_testFileManager,
    //                 &TestFileManager::exportPictureCollectionHandler);

    //// Window ask FileManager for a dialogue export
    //QObject::connect(m_window,
    //                 &Window::exportDialogueSignal,
    //                 m_testFileManager,
    //                 &TestFileManager::exportDialogueHandler);

    //// Video remux
    //QObject::connect(m_window,
    //                 &Window::exportVideoRemuxSignal,
    //                 m_testFileManager,
    //                 &TestFileManager::exportVideoRemuxHandler);

    //// Audio remux
    //QObject::connect(m_window,
    //                 &Window::exportAudioRemuxSignal,
    //                 m_testFileManager,
    //                 &TestFileManager::exportAudioRemuxHandler);

    //// Subtitle remux
    //QObject::connect(m_window,
    //                 &Window::exportSubtitleRemuxSignal,
    //                 m_testFileManager,
    //                 &TestFileManager::exportSubtitleRemuxHandler);

    //// Currently only used to fill ComboBoxes with id's
    //QObject::connect(m_testFileManager,
    //                 &TestFileManager::fileChangedSignal,
    //                 m_window,
    //                 &Window::fileChangedHandler);

    //// Send chosen sub stream info to window
    //QObject::connect(m_testFileManager,
    //                 &TestFileManager::subDescriptionReceivedSignal,
    //                 m_window,
    //                 &Window::subDescriptionReceivedHandler);

    //// FileManager allows dialogue export
    //QObject::connect(m_testFileManager,
    //                 &TestFileManager::allowDialogueExportSignal,
    //                 m_window,
    //                 &Window::allowDialogueExportHandler);
}

void WindowTest::initTestCase_data()
{
}

void WindowTest::cleanupTestCase()
{
    /* Singleton magic has been casted here:
     * we have to "pull them out" after an integration test */
    ProgressBar::instance().progressBar()->setLayout(nullptr);
    ProgressBar::instance().progressBar()->setParent(nullptr);

    Console::instance().textEdit()->setLayout(nullptr);
    Console::instance().textEdit()->setParent(nullptr);

    delete m_window;
    delete m_testFileManager;
}

void WindowTest::init()
{
}

void WindowTest::cleanup()
{
}


void WindowTest::testOpenFile()
{
    QFETCH(QString, file_path);
    QFETCH(bool, valid);

    QSignalSpy fileChangedSpy { m_testFileManager, SIGNAL(fileChangedSignal(int,
                                                                            const QList<DialogueFromVideo::AudioInfo*>&,
                                                                            const QList<DialogueFromVideo::SubInfo*>&   )) };

    // Using custom test signals from the mock fileManager class
    QSignalSpy fileOpenedSpy { m_testFileManager, SIGNAL(openFileTestOpened()) };
    QSignalSpy fileRejectedSpy { m_testFileManager, SIGNAL(openFileTestRejected()) };

    // Set up mock path
    m_testFileManager->setTestPath(file_path);
    // Test the Window signal
    emit m_window->openFileSignal();
    // Unset mock path
    m_testFileManager->clearTestPath();

    QCOMPARE(fileOpenedSpy.count(), valid ? 1 : 0);
    QCOMPARE(fileRejectedSpy.count(), valid ? 0 : 1);
}

void WindowTest::testOpenFile_data()
{
    QTest::addColumn<QString>("file_path");
    QTest::addColumn<bool>("valid");

    QTest::newRow("bad_file") << "not-a-media-file.txt" << false;
    QTest::newRow("good_file") << "My Soul, Your Beats! ~GiDeMo Version~.mp3" << true;
}

} // namespace DialogueTest
