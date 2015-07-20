
add_executable(http_test Http_test.cpp)
target_link_libraries(http_test wallehttp wallenet wallesys)
add_executable(http_client http_client.cpp)
target_link_libraries(http_client wallehttp wallenet wallesys)