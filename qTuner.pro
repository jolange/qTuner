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

DEPENDPATH += src 
INCLUDEPATH +=

# Input
FORMS     += 
HEADERS   += src/UIMain.hpp        \
             src/OutputDevice.hpp
SOURCES   += src/main.cpp          \
             src/UIMain.cpp        \
             src/OutputDevice.cpp
RESOURCES += 
