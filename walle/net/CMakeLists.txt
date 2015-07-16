set(NET_SRC
	Buffer
	Addrinet
	EventLoop
	)
add_library(wallenet ${NET_SRC})
target_link_libraries(wallenet  wallesys pthread rt)
install(TARGETS wallenet DESTINATION lib)
file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/walle/net)
add_subdirectory(test)

