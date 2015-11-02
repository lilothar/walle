set(http_SRCS
  Httpserver
  Httpresponse
  HttpClientRequest
  HttpClientResponse
  HttpClient
  )
add_library(wallehttp ${http_SRCS})
target_link_libraries(wallehttp wallenet wallesys)
install(TARGETS wallehttp DESTINATION lib)
file(GLOB HTTPHEADERS "*.h")
install(FILES ${HTTPHEADERS} DESTINATION include/walle/net/http)
add_subdirectory(test)