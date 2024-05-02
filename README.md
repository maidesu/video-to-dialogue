# Building on Windows

## 0. Prerequisites

- Qt Creator 12.x.x
- Qt 6.6.x

> [Download Qt](https://www.qt.io/download)

## 1. Source code

```
git clone https://github.com/maidesu/video-to-dialogue.git
```

## 2. a) Building FFmpeg from source

### 0. Prerequisites

- MSYS2
- MinGW

MSYS2 packages:
```sh
pacman -S mingw-w64-x64_64-gcc
pacman -S mingw-w64-x86_64-yasm
pacman -S diffutils
pacman -S mingw-w64-x86_64-make
```

### 1. Fetching project submodules

```sh
cd video-to-dialogue
git submodule update --init --recursive
```

### 2. Configuring FFmpeg

```sh
./lib/ffmpeg/configure --enable-shared --target-os=mingw64
```

### 3. Building FFmpeg

```sh
mingw32-make.exe
```

## 2. b) Using prebuilt FFmpeg

### 1. Obtain the [shared prebuilt libraries](https://www.gyan.dev/ffmpeg/builds/) (release: [6.6.1](https://www.gyan.dev/ffmpeg/builds/packages/ffmpeg-6.1.1-full_build-shared.7z))

### 2. Copy the following files to their appropriate locations:

- *`ffmpeg-6.6.1-full_build-shared/include/*` to `lib/ffmpeg/include/*`*
- *`ffmpeg-6.6.1-full_build-shared/lib/*` to `lib/ffmpeg/lib/*`*

## 3. Configure project

### Developing in Qt Creator:

1. Open the project file `dialogue_from_video.pro` located in the root directory.

2. Select one of the kits to be used for compilation.

3. Click "Configure Project".

4. Use a profile and one of the options on the *bottom-left* to *build*, *debug* or *run* the project.

## 4. Finally, place the dynamic-link library inside the build directory

> Depending on Step 2, either:

a) TODO

- TODO

> or:

b) Copy the .dll files from the archive downloaded previously

- *`ffmpeg-6.6.1-full_build-shared/bin/*` to `build-dialogue_from_video-Desktop_Qt_6_6_2_MinGW_64_bit-Debug/debug/*`*
