
add_executable(http_test Http_test.cc)
target_link_libraries(http_test wallehttp wallenet wallesys)
add_executable(http_client http_client.cc)
target_link_libraries(http_client wallehttp wallenet wallesys)