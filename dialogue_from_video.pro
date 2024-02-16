TARGET = DialogueFromVideo

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/application.cpp \
    src/file/read.cpp \
    src/file/remux.cpp \
    src/file/write.cpp \
    src/fileinfo.cpp \
    src/main.cpp \
    src/pipe/pipeline.cpp \
    src/pipe/stage.cpp \
    src/settings.cpp \
    src/widgets/restartmessagebox.cpp \
    src/widgets/window.cpp \
    src/subtitle/subtitle.cpp

HEADERS += \
    include/application.hpp \
    include/common/console.hpp \
    include/common/exitcode.hpp \
    include/common/messenger.hpp \
    include/common/progress.hpp \
    include/common/progressbar.hpp \
    include/common/singleton.hpp \
    include/common/time.hpp \
    include/file/read.hpp \
    include/file/remux.hpp \
    include/file/write.hpp \
    include/fileinfo.hpp \
    include/pipe/pipe.hpp \
    include/pipe/pipeline.hpp \
    include/pipe/stage.hpp \
    include/settings.hpp \
    include/widgets/palettes/colors.hpp \
    include/widgets/palettes/darkpalette.hpp \
    include/widgets/palettes/lightpalette.hpp \
    include/widgets/restartmessagebox.hpp \
    include/widgets/window.hpp \
    include/subtitle/subtitle.hpp

INCLUDEPATH += \
    include \
    lib/ffmpeg-6.1.1-full_build-shared/include

LIBS += \
    -L"$$PWD/lib/ffmpeg-6.1.1-full_build-shared/lib"

LIBS += \
    -lavformat \
    -lavcodec \
    -lavutil

TRANSLATIONS += \
    resource/translations/hu_HU.ts \
    resource/translations/en_US.ts

CONFIG += lrelease
CONFIG += embed_translations

RESOURCES += \
    resource/dialogue_from_video.qrc

QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wstrict-aliasing -pedantic -Werror

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
