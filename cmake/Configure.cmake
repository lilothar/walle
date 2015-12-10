
set(SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
include (${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)
#check_type_size(int WALLE_INT_SIZE)
#message(STATUS  "int type size: ${WALLE_INT_SIZE}")
message(STATUS  "system: ${CMAKE_SYSTEM}")
message(STATUS  "processer: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS  "system version: ${CMAKE_SYSTEM_VERSION}")

include(${CMAKE_ROOT}/Modules/TestBigEndian.cmake) 

test_big_endian(B) 
if(B) 
	set(SYSTEM_BIGEND 1) 
else () 
	set(SYSTEM_BIGEND 0) 
endif()

try_run(PAGE_SIZE_GET PAGE_SIZE_GET_BUILD
            ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp
            ${PROJECT_SOURCE_DIR}/cmake/getpagesize.cc
            CMAKE_FLAGS -g 
            RUN_OUTPUT_VARIABLE SYSTEM_PAGESIZE)
message(STATUS "system page size: ${SYSTEM_PAGESIZE}")

configure_file (
  "${PROJECT_SOURCE_DIR}/walle/config/config.h.in"
  "${PROJECT_BINARY_DIR}/walle/config/config.h"
  )


   
