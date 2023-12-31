<h1 align="center"><img src='https://raw.githubusercontent.com/docentYT/FFvid/main/src/Assets/icons/icon.svg' width='128'/></h1>

> Fast and easy-to-use video editor based on [FFmpeg](https://ffmpeg.org). Cross-platform, native GUI allows you to get the performance of pure FFmpeg without having to remember commands.

## ⚡ Fast
No re-encoding where possible is the fastest possible option for editing videos.

## 🖥️ Native feel
Using the C++ language and the multi-platform wxWidgets library generates a fast and clean application that launches in the blink of an eye.
| Windows 11 | GTK 3 |
|---|---|
| <img src="Screenshots/Windows.png" alt="Windows 11" width="512"/> | <img src="Screenshots/GTK3.png" alt="GTK 3" width="512"/> |

## ✨ Functions
`FFvid` currently offers:
- video trimming
- joining multiple videos into one
- adding a watermark to a video
- removing data (video, audio, subtitles, data streams) from file

## 🚀 How to install

### Windows
A simple installer has been prepared for Windows users and is available on the [Releases page](https://github.com/docentYT/FFvid/releases).

### Compile from source
You will need [CMake](https://cmake.org) and [wxWidgets](https://www.wxwidgets.org) installed. On systems other than Windows, the [fmt](https://fmt.dev) library is also needed. To build the project in debug configuration use:
```
cmake -S. -Bbuild
cmake --build build
```
or if you want to build in release configuration:
```
cmake -S. -Bbuild
cmake --build build --config Release
```
The main executable can be found in the `build/out/(Debug or Release)` directory.

To create an installer for Windows use `cpack -C Release` in `build` directory.
