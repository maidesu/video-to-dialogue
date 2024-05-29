TARGET = TestDialogue

QT += core testlib
QT += charts
QT -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt warn_on depend_includepath testcase
CONFIG += c++20

DEFINES += GIT_HASH="\\\"$$system(git -C \""$$_PRO_FILE_PWD_"\" rev-parse --short HEAD)\\\""
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_DEPRECATED_WARNINGS
DEFINES += PROJECT_PATH='\\"$$absolute_path($$_PRO_FILE_PWD_)\\"'

TEMPLATE = app

INCLUDEPATH += \
    include \
    ../include \
    ../lib/include

HEADERS += \
    include/consoletest.hpp \
    include/formatoptstest.hpp \
    include/messengertest.hpp \
    include/singletontest.hpp \
    include/timetest.hpp \
    include/progresstest.hpp \
    include/progressbartest.hpp \
    include/dialoguetest.hpp \
    include/windowtest.hpp \
    include/applicationtest.hpp \
    \
    ../include/application.hpp \
    ../include/audio/audio.hpp \
    ../include/common/averror.hpp \
    ../include/common/console.hpp \
    ../include/common/exitcode.hpp \
    ../include/common/formatopts.hpp \
    ../include/common/messenger.hpp \
    ../include/common/progress.hpp \
    ../include/common/progressbar.hpp \
    ../include/common/singleton.hpp \
    ../include/common/subtitlestructs.hpp \
    ../include/common/time.hpp \
    ../include/dialogue/dialogue.hpp \
    ../include/file/read.hpp \
    ../include/file/write.hpp \
    ../include/remux/remux.hpp \
    ../include/transcode/transcode.hpp \
    ../include/filemanager.hpp \
    ../include/frame/frame.hpp \
    ../include/pipe/pipe.hpp \
    ../include/pipe/pipeline.hpp \
    ../include/pipe/stage.hpp \
    ../include/settings.hpp \
    ../include/widgets/palettes/colors.hpp \
    ../include/widgets/palettes/darkpalette.hpp \
    ../include/widgets/palettes/lightpalette.hpp \
    ../include/widgets/restartmessagebox.hpp \
    ../include/widgets/waveformwidget.hpp \
    ../include/widgets/window.hpp \
    ../include/subtitle/subtitle.hpp

SOURCES += \
    src/main.cpp \
    src/consoletest.cpp \
    src/formatoptstest.cpp \
    src/messengertest.cpp \
    src/singletontest.cpp \
    src/timetest.cpp \
    src/progresstest.cpp \
    src/progressbartest.cpp \
    src/dialoguetest.cpp \
    src/windowtest.cpp \
    src/applicationtest.cpp \
    \
    ../src/application.cpp \
    ../src/audio/audio.cpp \
    ../src/common/formatopts.cpp \
    ../src/common/time.cpp \
    ../src/dialogue/dialogue.cpp \
    ../src/file/read.cpp \
    ../src/file/write.cpp \
    ../src/remux/remux.cpp \
    ../src/transcode/transcode.cpp \
    ../src/filemanager.cpp \
    ../src/frame/frame.cpp \
    ../src/pipe/pipeline.cpp \
    ../src/pipe/stage.cpp \
    ../src/settings.cpp \
    ../src/widgets/restartmessagebox.cpp \
    ../src/widgets/waveformwidget.cpp \
    ../src/widgets/window.cpp \
    ../src/subtitle/subtitle.cpp

LIBS += \
    -L"$$PWD/../lib/lib"

LIBS += \
    -lavformat \
    -lavcodec \
    -lavutil \
    -lswscale \
    -lswresample \
    -lavfilter

QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wstrict-aliasing -pedantic -Werror
