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
	LogStream
	Histogram
	Url
	MD5
	CountDown
	LogFile
	Asynclog.cpp
	)
add_library(wallesys ${SYS_SRC})
target_link_libraries(wallesys pthread rt)
install(TARGETS wallesys DESTINATION lib)
file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/walle/sys)
add_subdirectory(test)


