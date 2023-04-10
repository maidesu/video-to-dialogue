TARGET = DialogueFromVideo

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/application.cpp \
    src/fileinfo.cpp \
    src/main.cpp \
    src/pipe/pipeline.cpp \
    src/pipe/stage.cpp \
    src/settings.cpp \
    src/widgets/window.cpp

HEADERS += \
    include/application.hpp \
    include/common/console.hpp \
    include/common/messenger.hpp \
    include/common/singleton.hpp \
    include/fileinfo.hpp \
    include/pipe/pipe.hpp \
    include/pipe/pipeline.hpp \
    include/pipe/stage.hpp \
    include/settings.hpp \
    include/widgets/window.hpp

INCLUDEPATH += lib/ffmpeg-6.0-full_build-shared/include

LIBS += \
    -L"$$PWD/lib/ffmpeg-6.0-full_build-shared/lib"

LIBS += \
    -lavformat \
    -lavcodec \
    -lavutil

TRANSLATIONS += \
    resource/translations/dialogue_from_video_hu_HU.ts

CONFIG += lrelease
CONFIG += embed_translations

QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wstrict-aliasing -pedantic -Werror

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
