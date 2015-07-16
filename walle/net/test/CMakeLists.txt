add_executable(test_addr test_addr.cpp)
target_link_libraries(test_addr wallenet)

add_executable(test_loop_abort test_loop_abort.cpp)
target_link_libraries(test_loop_abort wallenet wallesys)

add_executable(test_loop_work test_loop_work.cpp)
target_link_libraries(test_loop_work wallenet wallesys)