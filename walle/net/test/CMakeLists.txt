add_executable(test_client_close test_client_close.cpp)
target_link_libraries(test_client_close wallenet wallesys)

add_executable(echo_client echo_client.cpp)
target_link_libraries(echo_client wallenet wallesys)

add_executable(echo_server echo_server.cpp)
target_link_libraries(echo_server wallenet wallesys)

add_executable(test_service test_service.cpp)
target_link_libraries(test_service wallenet wallesys)

add_executable(test_service_client test_service_client.cpp)
target_link_libraries(test_service_client wallenet wallesys)

add_executable(signal_test signal_test.cpp)
target_link_libraries(signal_test wallenet wallesys)