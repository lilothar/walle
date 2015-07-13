add_executable(config_utest Config_utest.cpp)
target_link_libraries(config_utest wallesys walleunit)

add_executable(stringutil_utest StringUtil_utest.cpp)
target_link_libraries(stringutil_utest wallesys walleunit)

add_executable(test_log test_log.cpp)
target_link_libraries(test_log wallesys)

add_executable(test_processinfo test_processinfo.cpp)
target_link_libraries(test_processinfo wallesys)
add_executable(test_md5 test_md5.cpp)
target_link_libraries(test_md5 wallesys)