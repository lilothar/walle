
set(UNIT_SRC 
	Ucollectoroutput.cpp
	Ucompileroutput.cpp
	Uhtmloutput.cpp
	Usource.cpp
	Usuite.cpp
	Utextoutput.cpp
	Utime.cpp
	Uutils.cpp
	)
	add_library(walleunit ${UNIT_SRC})
	target_link_libraries(walleunit rt)
	file(GLOB HEADERS "*.h")
	install(FILES ${HEADERS} DESTINATION include/walle/unit)
	install(TARGETS walleunit DESTINATION lib)
	
	add_subdirectory(test)