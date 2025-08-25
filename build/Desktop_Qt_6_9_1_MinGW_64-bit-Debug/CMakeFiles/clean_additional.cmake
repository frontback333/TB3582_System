# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\System_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\System_autogen.dir\\ParseCache.txt"
  "System_autogen"
  )
endif()
