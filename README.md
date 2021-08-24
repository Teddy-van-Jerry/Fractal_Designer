# Fractal Designer
This is an application to make fractal videos based on Qt(6.2.0), C++ and FFmpeg.

## Version Information
This is version 6.0.9 updated in 2021/08/02.

### What's new in beta2 version than beta1 version
- Add **Linux** support (tested on Ubuntu 20 using Qt 6.2.0 and Qt 6.1.2).
- Add terminal command `playvideo`.

> Amazed to find effieiency on Ubuntu 20 is twice of that on Windows 10 under the same circumstance!

### Please note
This version only supports source code and you need to compile it yourself.
This shall be the final beta version and the next release version will be the official 6.0.9 version (support on Windows and Linux).

Visit our [Fractal Designer Website](https://frd.teddy-van-jerry.org) for more information.
The [doxygen pages](https://frd.teddy-van-jerry.org/doxygen) are generated based on beta1 version and has not been upgraded to beta2 version.

## Build the Project
Besides the build version for Windows (using Qt 6.2.0), you can compile it yourself by following steps:

- Build project `include/qscintilla.pro`. On Windows, build with both Debug and Release mode; on Linux, build with Release mode.

- Open project `src/Fractal_Designer.pro` and *release translations* (lrelease).
In Qt Creator, it is in `Tools->External->Qt Languist`.

- Build `src/Fractal_Designer.pro` with the same kit as that of building QScintilla.

- Get **ffmpeg**.
  - For Windows, Download *shared* version of [ffmpeg](http://www.ffmpeg.org/download.html) and put all contents of directory `bin` it in the right directory. (They are `bin/win/ffmpeg.exe`, `bin/win/ffplay.exe` and `dll` dependencies.)
  - For Linux, download ffmpeg using `sudo apt install ffmpeg`.

- To distribute the project, you need to
  - Build in release mode.
  - Deploy with `windeployqt`, `macdeployqt`, `linuxdeployqt` or other methods to get the dependency files.
  - Copy the release dynamic library file of QScintilla (`qscintilla2_qt6.dll` on Windows, `libqscintilla2_qt6.so` and its symbolic links on Linux) to directory `bin` and delete directory `lib`, `scintilla`, `include`, `tmp`.

## Use Fractal Designer

Type `help` in the terminal to get the list of all commands supported by Fractal Designer 6.0.9.
Use the sample file to have a quick look at how the `frd` language is like.
Details about `frd` language will be updated in the near future.

### FRD Script
Here is the basic syntax of `frd` script:
- `frd` language is case-sensitive.
- Variable names are started with `$`, function names are started with `%`  and class names are started with `@`.
- Each statement is ended with `;` or a block (i.e. `{}`).
- Comments follow the `C++` rule (i.e. `//` and `/* */`).
- No custom class name is allowed in this version.
- Members of a class are specified also.
- No quote mark is needed outside `@string` value.
- To assign a `@formula` value, use `:=` instead of `=`.
- Currently a value can only be set during initialization. (Which seems to a bug of `QJson`)
- Variables is only valid in its scope block.
- `@number` values are acctually stored as complex numbers.
- `@array` stores `@number` and `@list` stores `@string`.
- `@array` and `@list` values are set by using `,` to divide different elements. No `{}` should be used.
- Function now only support `%CONFIGURE`.

> Note:
> - Non Ascii characters can lead to the misplace of syntax highlight of variable, function and class names.
> - Try to avoid syntax error or mathematics error. (For example, `Ts` in route should between 0 to 1 and `Widths` should not be negative.)
> The behavoir of the interpreter is undefined faced with these circumstances. *[This will be improved in the future.]*

### FRD Terminal
- Currently you just need to use the **Editor** tab.
- `Ctrl + O` to open `frd` files. (The default directory will have a sample file.)
- Change the *file location* and *image size* information of sample file according to your need.
- Enter `save` in terminal or use `Ctrl + S` to save your changes.
- Enter `run` to interpret the script (Every time you change the file you need to `run` to get the data updated.)
- Enter `preview` (or `F12`) to view the preview image and `closepreview` to close the preview window.
- Enter `createimages` (or `Ctrl + R`) to create images. This process can be time-consuming.
- Enter `createvideo` to create the video. This process is much faster than that of creating images but the window will not respond until this process finishes.
- Enter `playvideo` to watch the video you generated using *ffplay*.
- Other useful commands:
  - `help` to view all commands. This can help you choose what command and argument to use.
  - `clear` to clear the screen.
  - `history` to view the command history.
  - `info` to view data stored in json format.
  - `!!` to repeat the previous command. 

## TODOs in Version 6.0
- Validity Check in `frd` script.
- Improve error indication of interpreter.
- Add termination control in preview, creating images and creating video.
- Add more terminal commands.

## Sample Videos

| Video Name               | Watch on YouTube             | Watch on Bilibili           |
| :-:                      | :-:                          | :-:                         |
| Mandelbrot Set 1         | https://youtu.be/LU9eL1-9V0E | https://b23.tv/BV1TU4y147ta |
| Mandelbrot Set 2         | -                            | https://b23.tv/BV1qX4y1P74d |
| Mandelbrot Set 3         | -                            | https://b23.tv/BV17K4y1J7XM |
| Mandelbrot Set 4         | -                            | https://b23.tv/BV1Hy4y1T7MD |
| Julia Set 1              | -                            | https://b23.tv/BV1wB4y1N7pU |
| Burning Ship Fractal 1   | -                            | https://b23.tv/BV1RA41157kJ |
| Newton Fractal 1         | -                            | https://b23.tv/BV1Tb4y1D7oN |
| FRD 6.0.9 Ubuntu 20 Test | -                            | https://b23.tv/BV1CL411n7F1 |

> More videos are being uploaded.

## Download Release
This beta2 version only provides build version for Windows.
For other systems, please compile yourself.
