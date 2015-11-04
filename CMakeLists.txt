cmake_minimum_required(VERSION 2.6)

project(walle)

set (PROJECT_VERSION_MAJOR 1) 
set (PROJECT_VERSION_MINOR 0)

set(CXX_FLAGS
 -g
 -O0
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

#check c++11 start 
include (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake) 
include(${CMAKE_ROOT}/Modules/CheckCXXCompilerFlag.cmake) 
include(${CMAKE_ROOT}/Modules/TestBigEndian.cmake) 

test_big_endian(B) 
if(B) 
	set(SYSTEM_BIGEND 1) 
else () 
	set(SYSTEM_BIGEND 0) 
endif()

option(USE_CXX11 "compile with c++11" ON)
if (USE_CXX11) 
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	if (COMPILER_SUPPORTS_CXX11) 
		set(CXX_FLAGS "${CXX_FLAGS} --std=c++11")
		option(STD_CXX11 "compile with c++11" ON)
		message(STATUS "compile with c++11 " "ON") 
	else (COMPILER_SUPPORTS_CXX11) 
		option(STD_CXX11 "compile with c++11" OFF)
		message(STATUS "compile not support c++11 ") 
	endif(COMPILER_SUPPORTS_CXX11) 
else (USE_CXX11) 
	message(STATUS "compile with c++11 " "OFF") 
endif (USE_CXX11)

#check c++11 end

#check headers start
#check headers end
#check lib start
#check lib end


#build type start
set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo)

if(${BUILD_TYPE} MATCHES "DEBUG") 
	set(CMAKE_BUILD_TYPE Debug)
	option(WALLE_DEBUG "lib debug flag" ON) 
elseif (${BUILD_TYPE} MATCHES "RELEASE") 
	set(CMAKE_BUILD_TYPE Release) 
else () 
	option(WALLE_DEBUG "lib debug flag" ON)
	set(CMAKE_BUILD_TYPE RelWithDebInfo) 
endif() 

message(STATUS "program build type " ${CMAKE_BUILD_TYPE}) 

#build type end 

# install prefix start
if(PREFIX)	
	set(CMAKE_INSTALL_PREFIX ${PREFIX})
endif(PREFIX)
message(STATUS "program will be install to " ${CMAKE_INSTALL_PREFIX})
# install prefix end
message(STATUS "create config file") 
#init config start 
add_subdirectory(walle/config) 
#init config end

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
include_directories(.)
include_directories(${CMAKE_BINARY_DIR}) #for config.h
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(walle/sys)
add_subdirectory(walle/net)
add_subdirectory(walle/unit)
add_subdirectory(walle/ext)
add_subdirectory(walle/algo)
