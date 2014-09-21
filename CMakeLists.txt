PROJECT(qTuner)
set(PROJECTVERSION 0.3b)

FIND_PACKAGE(Qt4 COMPONENTS QTCORE QTGUI QTMULTIMEDIA)

cmake_minimum_required(VERSION 3.0)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

configure_file (
  "src/config.hpp.in"
  "${PROJECT_BINARY_DIR}/config.hpp"
)

SET(SOURCES
   src/main.cpp
   src/UIMain.cpp
   src/FFTDevice.cpp
   src/NoteInfo.cpp
   src/Tuning.cpp
   src/winMath.cpp
)
SET(QT_HEADERS
   src/FFTDevice.hpp
   src/NoteInfo.hpp
   src/UIMain.hpp
)
SET(FORMS
   src/UIMain.ui
)
SET(RESOURCES
   resources/qTuner.qrc
)

INCLUDE(${QT_USE_FILE})
QT4_WRAP_CPP     (HEADERS_MOC   ${QT_HEADERS})
QT4_WRAP_UI      (FORMS_HEADERS ${FORMS})
QT4_ADD_RESOURCES(RESOURCES_RCC ${RESOURCES})

ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(qTuner
   ${SOURCES}
   ${HEADERS_MOC}
   ${FORMS_HEADERS}
   ${RESOURCES_RCC}
)
TARGET_LINK_LIBRARIES(qTuner
   ${QT_LIBRARIES}
)

