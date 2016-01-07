set(NETSRC
	Acceptor
	Addrinet
	Buffer
	Channel
	Connector
	Eventloop
	Eventloopthread
	Eventloopthreadpool
	Netcount
	Poller
	Socket
	Tcpclient
	TcpConnection
	Tcpserver
	Timer
	Timertask
	Service
	TimerWheel
	)
	add_library(wallenetStatic STATIC ${NETSRC})
	target_link_libraries(wallenetStatic wallesysStatic)
	set_target_properties(wallenetStatic PROPERTIES OUTPUT_NAME wallenet)
	
	add_library(wallenetShared SHARED ${NETSRC})
	target_link_libraries(wallenetShared wallesysShared)
	set_target_properties(wallenetShared PROPERTIES OUTPUT_NAME wallenet)

	install(TARGETS wallenetStatic wallenetShared DESTINATION lib)
	file(GLOB NETHEADERS "*.h")
	install(FILES ${NETHEADERS} DESTINATION include/walle/net)

	add_subdirectory(test)
	add_subdirectory(http)
	
