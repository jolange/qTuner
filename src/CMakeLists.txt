
include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file (
  "config.hpp.in"
  "${PROJECT_BINARY_DIR}/src/config.hpp"
  )

set(SOURCES
  main.cpp
  UIMain.cpp
  FFTDevice.cpp
  NoteInfo.cpp
  Tuning.cpp
  winMath.cpp
  )
set(QT_HEADERS
  FFTDevice.hpp
  NoteInfo.hpp
  UIMain.hpp
  )
set(FORMS
  UIMain.ui
  )
set(RESOURCES
  ${CMAKE_SOURCE_DIR}/resources/qTuner.qrc
  )

qt5_wrap_cpp     (HEADERS_MOC   ${QT_HEADERS})
qt5_wrap_ui      (FORMS_HEADERS ${FORMS})
qt5_add_resources(RESOURCES_RCC ${RESOURCES})

add_executable(qTuner
  ${SOURCES}
  ${HEADERS_MOC}
  ${FORMS_HEADERS}
  ${RESOURCES_RCC}
  )

target_link_libraries(qTuner
  Qt5::Widgets
  Qt5::Multimedia
  )

INSTALL(TARGETS qTuner
  RUNTIME DESTINATION bin)
