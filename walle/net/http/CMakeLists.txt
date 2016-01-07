set(http_SRCS
  Httpserver
  Httpresponse
  HttpClientRequest
  HttpClientResponse
  HttpClient
  )
add_library(wallehttpStatic STATIC ${http_SRCS})
target_link_libraries(wallehttpStatic wallenetStatic wallesysStatic)
set_target_properties(wallehttpStatic PROPERTIES OUTPUT_NAME wallehttp)
add_library(wallehttpShared SHARED ${http_SRCS})
target_link_libraries(wallehttpShared wallenetShared wallesysShared)
set_target_properties(wallehttpShared PROPERTIES OUTPUT_NAME wallehttp)
install(TARGETS wallehttpStatic wallehttpShared DESTINATION lib)
file(GLOB HTTPHEADERS "*.h")
install(FILES ${HTTPHEADERS} DESTINATION include/walle/net/http)
add_subdirectory(test)