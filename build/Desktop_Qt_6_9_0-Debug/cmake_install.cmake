# Install script for directory: /home/qubesat/Projects/qt-sensors-bmp280-i2c

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/tmp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/bin/aarch64-linux-gnu-objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqt6-sensors-bmp280-i2c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqt6-sensors-bmp280-i2c.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqt6-sensors-bmp280-i2c.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/qubesat/Projects/qt-sensors-bmp280-i2c/build/Desktop_Qt_6_9_0-Debug/libqt6-sensors-bmp280-i2c.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqt6-sensors-bmp280-i2c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqt6-sensors-bmp280-i2c.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqt6-sensors-bmp280-i2c.so"
         OLD_RPATH "/opt/Qt/6.9.0/gcc_arm64/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/bin/aarch64-linux-gnu-strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libqt6-sensors-bmp280-i2c.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/Qt/6.9.0/gcc_arm64/include/QBMP280-I2C/qbmp280_p.h;/opt/Qt/6.9.0/gcc_arm64/include/QBMP280-I2C/QBMP280_global.h;/opt/Qt/6.9.0/gcc_arm64/include/QBMP280-I2C/qbmp280.h;/opt/Qt/6.9.0/gcc_arm64/include/QBMP280-I2C/qbmp280backend.h;/opt/Qt/6.9.0/gcc_arm64/include/QBMP280-I2C/qbmp280reading.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/opt/Qt/6.9.0/gcc_arm64/include/QBMP280-I2C" TYPE FILE FILES
    "/home/qubesat/Projects/qt-sensors-bmp280-i2c/qbmp280_p.h"
    "/home/qubesat/Projects/qt-sensors-bmp280-i2c/QBMP280_global.h"
    "/home/qubesat/Projects/qt-sensors-bmp280-i2c/qbmp280.h"
    "/home/qubesat/Projects/qt-sensors-bmp280-i2c/qbmp280backend.h"
    "/home/qubesat/Projects/qt-sensors-bmp280-i2c/qbmp280reading.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/opt/Qt/6.9.0/gcc_arm64/lib/libqt6-sensors-bmp280-i2c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/Qt/6.9.0/gcc_arm64/lib/libqt6-sensors-bmp280-i2c.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/opt/Qt/6.9.0/gcc_arm64/lib/libqt6-sensors-bmp280-i2c.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/Qt/6.9.0/gcc_arm64/lib/libqt6-sensors-bmp280-i2c.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/opt/Qt/6.9.0/gcc_arm64/lib" TYPE SHARED_LIBRARY FILES "/home/qubesat/Projects/qt-sensors-bmp280-i2c/build/Desktop_Qt_6_9_0-Debug/libqt6-sensors-bmp280-i2c.so")
  if(EXISTS "$ENV{DESTDIR}/opt/Qt/6.9.0/gcc_arm64/lib/libqt6-sensors-bmp280-i2c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/Qt/6.9.0/gcc_arm64/lib/libqt6-sensors-bmp280-i2c.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/opt/Qt/6.9.0/gcc_arm64/lib/libqt6-sensors-bmp280-i2c.so"
         OLD_RPATH "/opt/Qt/6.9.0/gcc_arm64/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/bin/aarch64-linux-gnu-strip" "$ENV{DESTDIR}/opt/Qt/6.9.0/gcc_arm64/lib/libqt6-sensors-bmp280-i2c.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/opt/Qt/6.9.0/gcc_arm64/plugins/sensors/libqt6-sensors-bmp280-i2c-plugin.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/Qt/6.9.0/gcc_arm64/plugins/sensors/libqt6-sensors-bmp280-i2c-plugin.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/opt/Qt/6.9.0/gcc_arm64/plugins/sensors/libqt6-sensors-bmp280-i2c-plugin.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/opt/Qt/6.9.0/gcc_arm64/plugins/sensors/libqt6-sensors-bmp280-i2c-plugin.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/opt/Qt/6.9.0/gcc_arm64/plugins/sensors" TYPE SHARED_LIBRARY FILES "/home/qubesat/Projects/qt-sensors-bmp280-i2c/build/Desktop_Qt_6_9_0-Debug/libqt6-sensors-bmp280-i2c-plugin.so")
  if(EXISTS "$ENV{DESTDIR}/opt/Qt/6.9.0/gcc_arm64/plugins/sensors/libqt6-sensors-bmp280-i2c-plugin.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/opt/Qt/6.9.0/gcc_arm64/plugins/sensors/libqt6-sensors-bmp280-i2c-plugin.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/opt/Qt/6.9.0/gcc_arm64/plugins/sensors/libqt6-sensors-bmp280-i2c-plugin.so"
         OLD_RPATH "/opt/Qt/6.9.0/gcc_arm64/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/bin/aarch64-linux-gnu-strip" "$ENV{DESTDIR}/opt/Qt/6.9.0/gcc_arm64/plugins/sensors/libqt6-sensors-bmp280-i2c-plugin.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
  file(WRITE "/home/qubesat/Projects/qt-sensors-bmp280-i2c/build/Desktop_Qt_6_9_0-Debug/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
