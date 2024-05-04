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
    ../include/common/messenger.hpp

SOURCES += \
    src/main.cpp \
    src/consoletest.cpp \
    src/formatoptstest.cpp \
    ../src/common/formatopts.cpp \
    src/messengertest.cpp

QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wstrict-aliasing -pedantic -Werror
