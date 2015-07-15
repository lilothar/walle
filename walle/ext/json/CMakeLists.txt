file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/walle/ext/json)

add_subdirectory(test)