# Fractal Designer
This is an application to make fractal videos based on Qt(6.2.0), C++ and FFmpeg.

## Version Information
This is version 6.0.9 updated in 2021/08/01.

**What's new in beta1 version than alpha version**:
- Solve bugs listed in [6.0.9 Alpha Release](https://github.com/T-V-J/Fractal_Designer/releases/tag/v6.0.9-alpha).
- Improve efficiency by up to 100% than alpha version.


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
