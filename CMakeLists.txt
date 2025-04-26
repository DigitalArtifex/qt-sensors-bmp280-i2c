cmake_minimum_required(VERSION 3.16)

include(CMakePackageConfigHelpers)

project(QBMP280-I2C LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(USE_SYSTEM_PATH OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
  Core
  Sensors
)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
  Core
  Sensors
)

if(USE_SYSTEM_PATH)
  set(INSTALL_PREFIX "/usr")
  message("Setting Install Path to /usr")
else()
  message("Setting Install Path to CMAKE_PREFIX_PATH")
  if("${CMAKE_PREFIX_PATH}" MATCHES ";|:")
    list(GET CMAKE_PREFIX_PATH 0 INSTALL_PREFIX)
  else()
    message("--${CMAKE_PREFIX_PATH}")
    set(INSTALL_PREFIX "${CMAKE_PREFIX_PATH}")
  endif()
endif()

if("${INSTALL_PREFIX}" MATCHES "/lib/cmake")
  string(REPLACE "/lib/cmake" "" INSTALL_PREFIX "${INSTALL_PREFIX}")
  set(INSTALL_LIB_PATH "${INSTALL_PREFIX}/lib")
  set(INSTALL_INCLUDE_PATH "${INSTALL_PREFIX}/include")
else()
  set(INSTALL_LIB_PATH "${INSTALL_PREFIX}/lib")
  set(INSTALL_INCLUDE_PATH "${INSTALL_PREFIX}/include")
endif()

#setup lib
set(OUTPUT_NAME "qt${QT_VERSION_MAJOR}-sensors-bmp280-i2c")

add_library(${OUTPUT_NAME} SHARED
  qbmp280_p.h
  QBMP280_global.h
  qbmp280.h
  qbmp280backend.h
  qbmp280reading.h
  qbmp280reading.cpp
  qbmp280.cpp
  qbmp280backend.cpp
)

set(INSTALL_HEADERS
  qbmp280_p.h
  QBMP280_global.h
  qbmp280.h
  qbmp280backend.h
  qbmp280reading.h
)

target_link_libraries(${OUTPUT_NAME} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Sensors
  i2c
)

target_compile_definitions(${OUTPUT_NAME} PRIVATE QBMP28__LIBRARY)

#setup lib install
target_sources(${OUTPUT_NAME} PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_SOURCE_DIR} FILES ${INSTALL_HEADERS})

install(
  TARGETS ${OUTPUT_NAME}
  FILE_SET
  HEADERS
  DESTINATION ${INSTALL_PREFIX}/include/QBMP280-I2C
)
message("Headers Install Location: ${INSTALL_PREFIX}/include/QBMP280-I2C")

install(
  TARGETS ${OUTPUT_NAME}
  LIBRARY
  DESTINATION ${INSTALL_PREFIX}/lib/
  NAMELINK_COMPONENT
)
message("Library Install Location: ${INSTALL_PREFIX}/lib")

#setup plugin
set(PLUGIN_NAME "qt${QT_VERSION_MAJOR}-sensors-bmp280-i2c-plugin")

add_library(${PLUGIN_NAME} SHARED
  qbmp280_p.h
  QBMP280_global.h
  qbmp280.h
  qbmp280backend.h
  qbmp280reading.h
  qbmp280reading.cpp
  qbmp280plugin.h
  qbmp280.cpp
  qbmp280backend.cpp
  plugin.json
)

target_link_libraries(${PLUGIN_NAME} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Sensors
  i2c
)

#setup plugin install
install(
  TARGETS ${PLUGIN_NAME}
  LIBRARY
  DESTINATION ${INSTALL_PREFIX}/plugins/sensors/
  NAMELINK_COMPONENT
)

message("Plugin Install Location: ${INSTALL_PREFIX}/plugins/sensors/")
