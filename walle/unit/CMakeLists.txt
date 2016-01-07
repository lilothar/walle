
set(UNIT_SRC 
	Ucollectoroutput
	Ucompileroutput
	Uhtmloutput
	Usource
	Usuite
	Utextoutput
	Utime
	Uutils
	)
	add_library(walleunitStatic STATIC ${UNIT_SRC})
	target_link_libraries(walleunitStatic rt)
	set_target_properties(walleunitStatic PROPERTIES OUTPUT_NAME walleunit)
	
	add_library(walleunitShared SHARED ${UNIT_SRC})
	target_link_libraries(walleunitShared rt)
	set_target_properties(walleunitShared PROPERTIES OUTPUT_NAME walleunit)

	file(GLOB HEADERS "*.h")
	install(FILES ${HEADERS} DESTINATION include/walle/unit)
	install(TARGETS walleunitStatic walleunitShared DESTINATION lib)
	
	add_subdirectory(test)
