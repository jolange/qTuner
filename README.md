#![icon](./resources/qTuner48x48.png)  qTuner
[Qt4](https://qt-project.org/) based Guitar Tuner, using your computer's microphone

&copy; 2014 Johannes Lange,
for **License** see [LICENSE file](LICENSE)

#### Getting qTuner
- for Windows you can just get a precompiled [binary](https://github.com/johannes-lange/qTuner/releases/download/v0.1/qTuner-bin.zip)
  (you might have to install the [MSVC++ libraries](http://www.microsoft.com/en-gb/download/details.aspx?id=5555))
- or [build](#building) manually for any operating system

#### Building
- **Prerequisites**: [Qt4](https://qt-project.org/), microphone (built-in, headset, ...)
- **Linux**: In the base-path:
<pre><code>$ qmake
$ make
$ ./bin/qTuner</pre></code>
- **Windows**: in Visual Studio Command prompt
<pre><code>$ qmake
$ nmake release</pre></code>

##### [Release](https://github.com/johannes-lange/qTuner/releases) notes:
- development Version 0.2b: tuning presets, saving settings, [evaluation threshold]
- 2014-04-03 Version 0.1: first release
