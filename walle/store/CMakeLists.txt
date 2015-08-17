set(STORESRC
	util
	thread
	regex
	db
	file
	compress
	)
	add_library(wallestore ${STORESRC})
	target_link_libraries(wallestore z rt pthread)

	install(TARGETS wallestore DESTINATION lib)
	file(GLOB NETHEADERS "*.h")
	install(FILES ${NETHEADERS} DESTINATION include/walle/store)
add_subdirectory(test)