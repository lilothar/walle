
if(NOT CMAKE_COMPILER_IS_GNUCXX)
    message(FATAL_ERROR "compiler is not GNU")
else ()
    message(STATUS "compiler is GNU")
endif()

set(CMAKE_CXX_COMPILER "g++")

set(CXX_FLAGS
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Werror
 #-Wno-unused-parameter
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
  -MMD
 # -std=c++0x
 -rdynamic
 )
 
include(${CMAKE_ROOT}/Modules/CheckCXXCompilerFlag.cmake)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X) 
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)

if(NOT COMPILER_SUPPORTS_CXX0X)
    message(FATAL_ERROR "compiler do not supoort c++0x")
endif()

if(COMPILER_SUPPORTS_CXX11 AND USE_C11)
    set(CXX_FLAGS "${CXX_FLAGS} -std=c++11")
else()
    set(CXX_FLAGS "${CXX_FLAGS} -std=c++0x")
endif()
 string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
 set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
 set(CMAKE_CXX_FLAGS_RELEASE "-O2")