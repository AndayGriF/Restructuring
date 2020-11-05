# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Restruction_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Restruction_autogen.dir\\ParseCache.txt"
  "Restruction_autogen"
  )
endif()
