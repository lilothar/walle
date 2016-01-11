
set(SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
include (${CMAKE_ROOT}/Modules/CheckTypeSize.cmake)
#check_type_size(int WALLE_INT_SIZE)
#message(STATUS  "int type size: ${WALLE_INT_SIZE}")
message(STATUS  "system: ${CMAKE_SYSTEM}")
message(STATUS  "processer: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS  "system version: ${CMAKE_SYSTEM_VERSION}")

include(${CMAKE_ROOT}/Modules/TestBigEndian.cmake) 

test_big_endian(WORDS_BIGENDIAN) 
if(WORDS_BIGENDIAN) 
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

try_run(PLATE_GET PLATE_GET_BUILD
            ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp
            ${PROJECT_SOURCE_DIR}/cmake/getplateform.cc
            CMAKE_FLAGS -g 
            RUN_OUTPUT_VARIABLE SYSTEM_PLATE)
message(STATUS "system plate form is: ${SYSTEM_PLATE}")

set(TRY_COMPILE_CODE "if(__builtin_expect(1<2, 1)) {}")
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/cmake/buildin.c ${PROJECT_BINARY_DIR}/check-builtin-expect.c)
TRY_COMPILE(HAVE_BUILTIN_EXPECT ${PROJECT_BINARY_DIR} 
            ${PROJECT_BINARY_DIR}/check-builtin-expect.c) 

set(TRY_COMPILE_CODE "int leading_zeros = __builtin_ctzll(4)")
CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/cmake/buildin.c ${PROJECT_BINARY_DIR}/check-builtin-ctz.c)
TRY_COMPILE(HAVE_BUILTIN_CTZ ${PROJECT_BINARY_DIR} 
            ${PROJECT_BINARY_DIR}/check-builtin-ctz.c) 
            
configure_file (
  "${PROJECT_SOURCE_DIR}/walle/config/config.h.in"
  "${PROJECT_BINARY_DIR}/walle/config/config.h"
  )


   
