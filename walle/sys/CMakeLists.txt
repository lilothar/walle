set(SYS_SRC
	Config
	Time
	StringUtil
	Filesystem
	SpinLock
	Cond
	Mutex
	Thread
	ReadSmallFile
	AppendFile
	Processinfo
	Logging
	LocalThread
	LogStream
	Histogram
	Url
	MD5
	CountDown
	LogFile
	Asynclog
	Properties
	)
add_library(wallesysStatic STATIC ${SYS_SRC})
target_link_libraries(wallesysStatic pthread rt)
set_target_properties(wallesysStatic PROPERTIES OUTPUT_NAME wallesys)

add_library(wallesysShared SHARED ${SYS_SRC})
target_link_libraries(wallesysShared pthread rt)
set_target_properties(wallesysShared PROPERTIES OUTPUT_NAME wallesys)

install(TARGETS wallesysStatic wallesysShared DESTINATION lib)
file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/walle/sys)
add_subdirectory(test)


