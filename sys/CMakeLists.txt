set(SYS_SRC
	Cond.cpp
	Config.cpp
	Exception.cpp
	Filesystem.cpp
	Mutex.cpp
	ReadSmallFile.cpp
	AppendFile.cpp
	Processinfo.cpp
	ReadSmallFile.cpp
	SpinLock.cpp
	StringUtil.cpp
	Thread.cpp
	Time.cpp
	EventLoop.cpp
	IOComponent.cpp
	EventFD.cpp
	
	)
add_library(wallesys ${SYS_SRC})
target_link_libraries(wallesys pthread rt)

install(TARGETS wallesys DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/walle/sys)

#add_subdirectory(tests)