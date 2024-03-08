# Building

## 0. Prerequisites

- Qt Creator 12.x.x
- Qt 6.6.x

[Download Qt](https://www.qt.io/download)

## 1. Source

```
git clone https://github.com/maidesu/video-to-dialogue.git
```

<br/>The following steps are only required for building FFmpeg from source:
```sh
cd video-to-dialogue
git submodule update --init --recursive
```

## 2. FFmpeg

- Obtain the [shared prebuilt libraries](https://www.gyan.dev/ffmpeg/builds/)

- Edit `dialogue_from_video.pro` and replace `PATH_TO_SHARED_BUILD` as follows:

```QML
INCLUDEPATH +=
    $$PWD/lib/PATH_TO_SHARED_BUILD/include

LIBS +=
    -L"$$PWD/lib/PATH_TO_SHARED_BUILD/lib"
```
