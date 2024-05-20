TARGET = DialogueFromVideo

QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

VERSION = 1.0.4.0

DEFINES += GIT_HASH="\\\"$$system(git -C \""$$_PRO_FILE_PWD_"\" rev-parse --short HEAD)\\\""

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/application.cpp \
    src/audio/audio.cpp \
    src/common/formatopts.cpp \
    src/common/time.cpp \
    src/dialogue/dialogue.cpp \
    src/file/read.cpp \
    src/file/write.cpp \
    src/remux/remux.cpp \
    src/transcode/transcode.cpp \
    src/filemanager.cpp \
    src/frame/frame.cpp \
    src/main.cpp \
    src/pipe/pipeline.cpp \
    src/pipe/stage.cpp \
    src/settings.cpp \
    src/widgets/restartmessagebox.cpp \
    src/widgets/waveformwidget.cpp \
    src/widgets/window.cpp \
    src/subtitle/subtitle.cpp

HEADERS += \
    include/application.hpp \
    include/audio/audio.hpp \
    include/common/averror.hpp \
    include/common/console.hpp \
    include/common/exitcode.hpp \
    include/common/formatopts.hpp \
    include/common/messenger.hpp \
    include/common/progress.hpp \
    include/common/progressbar.hpp \
    include/common/singleton.hpp \
    include/common/subtitlestructs.hpp \
    include/common/time.hpp \
    include/dialogue/dialogue.hpp \
    include/file/read.hpp \
    include/file/write.hpp \
    include/remux/remux.hpp \
    include/transcode/transcode.hpp \
    include/filemanager.hpp \
    include/frame/frame.hpp \
    include/pipe/pipe.hpp \
    include/pipe/pipeline.hpp \
    include/pipe/stage.hpp \
    include/settings.hpp \
    include/widgets/palettes/colors.hpp \
    include/widgets/palettes/darkpalette.hpp \
    include/widgets/palettes/lightpalette.hpp \
    include/widgets/restartmessagebox.hpp \
    include/widgets/waveformwidget.hpp \
    include/widgets/window.hpp \
    include/subtitle/subtitle.hpp

INCLUDEPATH += \
    include \
    lib/include

LIBS += \
    -L"$$PWD/lib/lib"

LIBS += \
    -lavformat \
    -lavcodec \
    -lavutil \
    -lswscale \
    -lswresample \
    -lavfilter

TRANSLATIONS += \
    resource/translations/hu_HU.ts \
    resource/translations/en_US.ts

CONFIG += lrelease
CONFIG += embed_translations

RESOURCES += \
    resource/dialogue_from_video.qrc

RC_ICONS = resource/img/icon.ico

QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wstrict-aliasing -pedantic -Werror

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
