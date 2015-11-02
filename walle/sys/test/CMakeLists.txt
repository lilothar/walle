add_executable(config_utest Config_utest.cc)
target_link_libraries(config_utest wallesys walleunit)

add_executable(stringutil_utest StringUtil_utest.cc)
target_link_libraries(stringutil_utest wallesys walleunit)

add_executable(test_log test_log.cc)
target_link_libraries(test_log wallesys)

add_executable(test_processinfo test_processinfo.cc)
target_link_libraries(test_processinfo wallesys)
add_executable(test_md5 test_md5.cc)
target_link_libraries(test_md5 wallesys)

add_executable(test_random test_random.cc)
target_link_libraries(test_random wallesys)

add_executable(test_string_to_sec test_string_to_sec.cc)
target_link_libraries(test_string_to_sec wallesys)


add_executable(test_cmdline test_cmdline.cc)
target_link_libraries(test_cmdline wallesys)

add_executable(test_config_mem test_config_mem.cc)
target_link_libraries(test_config_mem wallesys)

add_executable(asynclog_test asynclog_test.cc)
target_link_libraries(asynclog_test wallesys)