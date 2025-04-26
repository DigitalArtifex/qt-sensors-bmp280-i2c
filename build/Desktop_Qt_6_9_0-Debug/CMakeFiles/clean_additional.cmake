# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/qt6-sensors-bmp280-i2c-plugin_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/qt6-sensors-bmp280-i2c-plugin_autogen.dir/ParseCache.txt"
  "CMakeFiles/qt6-sensors-bmp280-i2c_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/qt6-sensors-bmp280-i2c_autogen.dir/ParseCache.txt"
  "qt6-sensors-bmp280-i2c-plugin_autogen"
  "qt6-sensors-bmp280-i2c_autogen"
  )
endif()
