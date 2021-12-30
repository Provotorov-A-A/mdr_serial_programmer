cmake_minimum_required(VERSION 3.14)

project(mdr_serial_programmer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core LinguistTools SerialPort REQUIRED)

set(TS_FILES mdr_serial_programmer_ru_RU.ts)

add_executable(mdr_serial_programmer
  src/main.cpp
  src/maindirector.h
  src/maindirector.cpp
  src/settings.h
  src/settings.cpp
  src/argparser.h
  src/argparser.cpp
  src/abstracthardwareinterface.h
  src/serial_settings.h
  ${TS_FILES}
)
target_link_libraries(mdr_serial_programmer Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::SerialPort)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
