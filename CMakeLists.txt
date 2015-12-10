cmake_minimum_required(VERSION 2.6)

project(walle)

set (PROJECT_VERSION_MAJOR 1) 
set (PROJECT_VERSION_MINOR 0)

include(${PROJECT_SOURCE_DIR}/cmake/UserOption.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/CheckCompiler.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Checkheaders.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Configure.cmake)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR}) #for config.h

#add_executable(hello test.cc)

add_subdirectory(walle/config)
add_subdirectory(walle/unit)
add_subdirectory(walle/sys)
add_subdirectory(walle/ext)
add_subdirectory(walle/net)
add_subdirectory(walle/algo)