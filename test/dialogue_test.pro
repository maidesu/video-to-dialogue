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
    ../lib/ffmpeg-6.1.1-full_build-shared/include

HEADERS += \
    include/consoletest.hpp \
    ../include/common/console.hpp

SOURCES += \
    src/main.cpp \
    src/consoletest.cpp

QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wstrict-aliasing -pedantic -Werror
