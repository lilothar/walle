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
	add_library(wallenet ${NETSRC})
	target_link_libraries(wallenet wallesys)

	install(TARGETS wallenet DESTINATION lib)
	file(GLOB NETHEADERS "*.h")
	install(FILES ${NETHEADERS} DESTINATION include/walle/net)

	add_subdirectory(test)
	add_subdirectory(http)
	
