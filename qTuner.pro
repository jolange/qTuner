TEMPLATE = app
TARGET   = qTuner
VERSION  = 0.1b


# for access in code
DEFINES += PROJECTVERSION=$$VERSION

# https://qt-project.org/doc/qt-5/qmake-variable-reference.html
DESTDIR = bin
BUILDDIR = build
OBJECTS_DIR = $${BUILDDIR}
MOC_DIR = $${BUILDDIR}
RCC_DIR = $${BUILDDIR}
UI_DIR = $${BUILDDIR}

QT += multimedia

#QMAKE_CXXFLAGS_RELEASE -= -O
#QMAKE_CXXFLAGS_RELEASE -= -O1
#QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS_RELEASE *= -O3


DEPENDPATH += src 
INCLUDEPATH +=

# Input
FORMS     += src/UIMain.ui
HEADERS   += src/UIMain.hpp        \
             src/FFTDevice.hpp     \
             src/NoteInfo.hpp      \
             src/winMath.hpp
SOURCES   += src/main.cpp          \
             src/UIMain.cpp        \
             src/FFTDevice.cpp     \
             src/NoteInfo.cpp      \
             src/winMath.cpp
RESOURCES += resources/qTuner.qrc
