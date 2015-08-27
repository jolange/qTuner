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
- **Prerequisites**: [Qt4](https://qt-project.org/)
- **Linux**: In the base-path:
<pre><code>$ qmake
$ make
$ ./bin/qTuner</pre></code>
- **Windows**: in Visual Studio Command prompt
<pre><code>$ qmake
$ nmake release</pre></code>
if you want a statically linked executable, you'll need a static Qt build

##### [Release](https://github.com/jolange/qTuner/releases) notes:
- development Version 0.3b: ukulele presets
- 2014-05-31 Version 0.2: tuning presets, saving settings, evaluation threshold
- 2014-04-03 Version 0.1: first release
