add_executable(test_client_close test_client_close.cc)
target_link_libraries(test_client_close wallenet wallesys)

add_executable(echo_client echo_client.cc)
target_link_libraries(echo_client wallenet wallesys)

add_executable(echo_server echo_server.cc)
target_link_libraries(echo_server wallenet wallesys)

add_executable(test_service test_service.cc)
target_link_libraries(test_service wallenet wallesys)

add_executable(test_service_client test_service_client.cc)
target_link_libraries(test_service_client wallenet wallesys)

add_executable(signal_test signal_test.cc)
target_link_libraries(signal_test wallenet wallesys)


add_executable(test_loop test_loop.cc)
target_link_libraries(test_loop wallenet)