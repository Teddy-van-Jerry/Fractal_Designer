# Fractal Designer
This is an application to make fractal videos based on Qt(6.2.0), C++ and FFmpeg.

## Version Information
This is version 6.0.9 updated in 2021/08/01.
Current version is now only available for Windows.
Release for Linux will be published in the near future.

Visit our [Fractal Designer Website](https://frd.teddy-van-jerry.org) for more information.

## Build the Project
Besides the build version for Windows (using Qt 6.2.0), you can compile it yourself by following steps:

- Build project `include/qscintilla.pro` with both Debug and Release mode.
Remember to uncheck *shadow build*.

- Open project `src/Fractal_Designer.pro` and *release translations* (lrelease).
In Qt Creator, it is in `Tools->External->Qt Languist`.

- Build `src/Fractal_Designer.pro` with the same kit as that of building QScintilla.
Also remember to uncheck *shadow build*.

- To distribute the project, you need to
  - Build in release mode.
  - Deploy with `windeployqt`, `macdeployqt`, `linuxdeployqt` or other methods to get the dependency files.
  - Copy the release dynamic library file of QScintilla (`qscintilla2_qt6.dll` in Windows) to directory `bin` and delete directory `lib`, `scintilla`, `include`, `tmp`.
  - Download [ffmpeg](http://www.ffmpeg.org/download.html) and put it in the right directory. (For Windows, it is `bin/win/ffmpeg.exe`.)

## Use Fractal Designer

Type `help` in the terminal to get the list of all commands supported by Fractal Designer 6.0.9.
Use the sample file to have a quick look at how the `frd` language is like.
Details about `frd` language will be updated in the near future.

## TODOs in Version 6.0
- Validity Check in `frd` script.
- Improve error indication of interpreter.
- Add termination control in preview, creating images and creating video.
- Add more terminal commands.

## Sample Videos

| Video Name              | Watch on YouTube             | Watch on Bilibili           |
| :-:                     | :-:                          | :-:                         |
| Mandelbrot Set 1        | https://youtu.be/LU9eL1-9V0E | https://b23.tv/BV1TU4y147ta |
| Mandelbrot Set 2        | -                            | https://b23.tv/BV1qX4y1P74d |
| Mandelbrot Set 3        | -                            | https://b23.tv/BV17K4y1J7XM |
| Mandelbrot Set 4        | -                            | https://b23.tv/BV1Hy4y1T7MD |
| Julia Set 1             | -                            | https://b23.tv/BV1wB4y1N7pU |
| Burning Ship Fractal 1  | -                            | https://b23.tv/BV1RA41157kJ |
| Newton Fractal 1        | -                            | https://b23.tv/BV1Tb4y1D7oN |

> More videos are being uploaded.

## Download Release
Now there is only release versions for Windows in this version.
