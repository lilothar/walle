cmake_minimum_required(VERSION 2.8)

project(walle)

set(CXX_FLAGS
 -g
 -O0
 # -DVALGRIND
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Werror
 -Wno-unused-parameter
 -Woverloaded-virtual
 -Wpointer-arith
 #-Wshadow
 -Wwrite-strings
 -march=native
  -MMD
 # -std=c++0x
 -rdynamic
 )

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
include_directories(.)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(walle/sys)
add_subdirectory(walle/net)
add_subdirectory(walle/unit)
add_subdirectory(walle/ext)
add_subdirectory(walle/algo)
#add_subdirectory(walle/store)
#add_subdirectory(walle/wlevel)