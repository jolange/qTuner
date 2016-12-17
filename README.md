#![icon](./resources/qTuner48x48.png)  qTuner
[Qt4](https://qt-project.org/) based Guitar Tuner, using your computer's microphone

Homepage: [http://jolange.github.io/#qTuner](http://jolange.github.io/#qTuner)

&copy; 2014 Johannes Lange,
for **License** see [LICENSE file](LICENSE)

![Screenshot](http://jolange.github.io/img/screenshots/linux.png)

#### Getting qTuner
- for Windows you can just get a precompiled [binary](https://github.com/jolange/qTuner/releases/download/v0.2/qTuner.exe)
  (you might have to install the [MSVC++ libraries](http://www.microsoft.com/en-gb/download/details.aspx?id=5555))
- or [build](#building) manually for any operating system

#### Building
- **Prerequisites**: [Qt4](https://qt-project.org/), CMake>=2.8
- **Linux**:

        mkdir build; cd build
        cmake ..
        # or cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/install_location
        make
        make install
- **Windows**: CMake-build not yet tested in Windows. The qmake version can be built in Visual Studio Command prompt
<pre><code>$ qmake
$ nmake release</pre></code>
if you want a statically linked executable, you'll need a static Qt build

##### Troubleshooting
If you are using ALSA on Linux, you might encounter this message,

    PulseAudioService: pa_context_connect() failed
    using null input device, none available

indicating that the packaged Qt version of your distribution is compiled without ALSA support.
You can [compile Qt](https://wiki.qt.io/Building_Qt_5_from_Git#Getting_the_source_code) with ALSA support on your own by adding the `-alsa` flag in the `configure` step.

When compiling qTuner, select your compiled Qt version by adding `-DCMAKE_PREFIX_PATH=<path>` to your `cmake` invocation, with `<path>` pointing to the `lib/cmake` directory of your installation (e.g. `/usr/local/Qt-5.5.1/lib/cmake`).

##### [Release](https://github.com/jolange/qTuner/releases) notes:
- development Version 0.3b: ukulele presets, cmake build, Qt5
- 2014-05-31 Version 0.2: tuning presets, saving settings, evaluation threshold
- 2014-04-03 Version 0.1: first release
