TARGET = TestDialogue

QT += core testlib
QT -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qt warn_on depend_includepath testcase
CONFIG += c++17

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_DEPRECATED_WARNINGS

TEMPLATE = app

INCLUDEPATH += \
    include \
    ../include \
    ../lib/include

HEADERS += \
    include/consoletest.hpp \
    ../include/common/console.hpp \
    include/formatoptstest.hpp \
    ../include/common/formatopts.hpp \
    include/messengertest.hpp \
    ../include/common/messenger.hpp \
    include/singletontest.hpp \
    ../include/common/singleton.hpp \
    include/timetest.hpp \
    ../include/common/time.hpp \
    include/progresstest.hpp \
    ../include/common/progress.hpp \
    include/progressbartest.hpp \
    ../include/common/progressbar.hpp \
    include/dialoguetest.hpp \
    ../include/dialogue/dialogue.hpp

SOURCES += \
    src/main.cpp \
    src/consoletest.cpp \
    src/formatoptstest.cpp \
    ../src/common/formatopts.cpp \
    src/messengertest.cpp \
    src/singletontest.cpp \
    src/timetest.cpp \
    ../src/common/time.cpp \
    src/progresstest.cpp \
    src/progressbartest.cpp \
    src/dialoguetest.cpp \
    ../src/dialogue/dialogue.cpp

QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wstrict-aliasing -pedantic -Werror
