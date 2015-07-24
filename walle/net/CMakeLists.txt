set(NETSRC
	Acceptor.cpp
	Addrinet.cpp
	Buffer.cpp
	Channel.cpp
	Connector.cpp
	Eventloop.cpp
	Eventloopthread.cpp
	Eventloopthreadpool.cpp
	Netcount.cpp
	Poller.cpp
	Socket.cpp
	Tcpclient.cpp
	TcpConnection.cpp
	Tcpserver.cpp
	Timer.cpp
	Timertask.cpp
	Service.cpp
	)
	add_library(wallenet ${NETSRC})
	target_link_libraries(wallenet wallesys)

	install(TARGETS wallenet DESTINATION lib)
	file(GLOB NETHEADERS "*.h")
	install(FILES ${NETHEADERS} DESTINATION include/walle/net)

	add_subdirectory(test)
	add_subdirectory(http)
	
