#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <netinet/tcp.h>
#include <string>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <walle/net/Socket.h>
#include <walle/net/Netcount.h>
#include <walle/sys/Logging.h>
#if defined(__clang__) || __GNUC_MINOR__ >= 6
#pragma GCC diagnostic push
#endif
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wold-style-cast"


namespace walle {
namespace net {

Socket::Socket(bool own) {
    _socketHandle = -1;
    _own = own;
}

/*
 * 析构函数
 */
Socket::~Socket() 
{
    if( _own) {
          shutdown();    
          close();
    }

}
void Socket::shutdown() {
	::shutdown(_socketHandle, SHUT_WR);
}
void Socket::close()
{
    if(_socketHandle == -1) {
        return ;
    }
	::close(_socketHandle);
    _socketHandle = -1;
}

/*
 * socket 句柄是否创建
 */
bool Socket::checkSocketHandle() {
	if (_socketHandle > 0) {
		return true;
	}
    if (_socketHandle == -1) { 
		if ((_socketHandle = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
       		 return false;
		}
		setSoBlocking();
    }
	return true;	
}

bool Socket::connect( AddrInet& addr) 
{
	int ret;
	if (!checkSocketHandle()) {
		return false;
	}
	 struct sockaddr* remoteaddr = ( struct sockaddr*)addr.getAddr(); 
    ret =::connect(_socketHandle, remoteaddr, sizeof(struct sockaddr));

    return ret;
}
bool Socket::bind( AddrInet& addr)  
{
	int ret;
	if (!checkSocketHandle()) {
		return false;
	}
	 struct sockaddr* serveraddr = ( struct sockaddr*)addr.getAddr();
	ret = ::bind(_socketHandle,serveraddr , sizeof(struct sockaddr));
	if(ret == 0) {
		return true;
	}
	return false;
}
bool Socket::listen(int backlog)
{
	if(backlog<=0) {
		backlog = 5;
	}
	return (0==::listen(_socketHandle, backlog));
}

int Socket::accept(AddrInet& peeraddr)

{
	 socklen_t len = (socklen_t)sizeof(struct sockaddr);
	 int connfd = ::accept(_socketHandle,
	 	(struct sockaddr*)peeraddr.getAddr(),&len);
	 if (connfd < 0) {
	     _savedErr = errno;
	     //log_error("accept error errno:%d",sock->_savedErr);
	 }
	 return connfd;

}
/*
 * 把socketHandle,及ipaddress设置到此socket中
 *
 * @param  socketHandle: socket的文件句柄
 * @param hostAddress: 服务器地址
 */

void Socket::setFd(int socketHandle) {
    _socketHandle = socketHandle;
}

/*
 * 返回文件句柄
 *
 * @return 文件句柄
 */
int Socket::getFd() {
    return _socketHandle;
}

/*
 * 写数据
 */
int Socket::write (const void *data, int len) {
    if (_socketHandle == -1) {
        return -1;
    }

    int res;
    do {
        res = ::write(_socketHandle, data, len);
    } while (res < 0 && errno == EINTR);
    return res;
}

/*
 * 读数据
 */
 bool Socket::getTcpInfo(struct tcp_info* tcpi) const
{
  socklen_t len = sizeof(*tcpi);
  bzero(tcpi, len);
  return ::getsockopt(_socketHandle, SOL_TCP, TCP_INFO, tcpi, &len) == 0;
}

bool Socket::getTcpInfoString(char* buf, int len) const
{
  struct tcp_info tcpi;
  bool ok = getTcpInfo(&tcpi);
  if (ok)
  {
    snprintf(buf, len, "unrecovered=%u "
             "rto=%u ato=%u snd_mss=%u rcv_mss=%u "
             "lost=%u retrans=%u rtt=%u rttvar=%u "
             "sshthresh=%u cwnd=%u total_retrans=%u",
             tcpi.tcpi_retransmits,  // Number of unrecovered [RTO] timeouts
             tcpi.tcpi_rto,          // Retransmit timeout in usec
             tcpi.tcpi_ato,          // Predicted tick of soft clock in usec
             tcpi.tcpi_snd_mss,
             tcpi.tcpi_rcv_mss,
             tcpi.tcpi_lost,         // Lost packets
             tcpi.tcpi_retrans,      // Retransmitted packets out
             tcpi.tcpi_rtt,          // Smoothed round trip time in usec
             tcpi.tcpi_rttvar,       // Medium deviation
             tcpi.tcpi_snd_ssthresh,
             tcpi.tcpi_snd_cwnd,
             tcpi.tcpi_total_retrans);  
  }
  return ok;
}
int Socket::read (void *data, int len) {
    if (_socketHandle == -1) {
        return -1;
    }

    int res;
    do {
        res = ::read(_socketHandle, data, len);
    } while (res < 0 && errno == EINTR);
    return res;
}

/*
 * 设置int类型的option
 */
bool Socket::setIntOption (int option, int value) {
    bool rc=false;
    if (checkSocketHandle()) {
        rc = (setsockopt(_socketHandle, SOL_SOCKET, option,
                         (const void *)(&value), sizeof(value)) == 0);
    }
    return rc;
}

/*
 * 设置time类型的option
 */
bool Socket::setTimeOption(int option, int milliseconds) {
    bool rc=false;
    if (checkSocketHandle()) {
        struct timeval timeout;
        timeout.tv_sec = (int)(milliseconds / 1000);
        timeout.tv_usec = (milliseconds % 1000) * 1000000;
        rc = (setsockopt(_socketHandle, SOL_SOCKET, option,
                         (const void *)(&timeout), sizeof(timeout)) == 0);
    }
    return rc;
}

bool Socket::setSoLinger(bool doLinger, int seconds) {
    bool rc=false;
    struct linger lingerTime;
    lingerTime.l_onoff = doLinger ? 1 : 0;
    lingerTime.l_linger = seconds;
    if (checkSocketHandle()) {
        rc = (setsockopt(_socketHandle, SOL_SOCKET, SO_LINGER,
                         (const void *)(&lingerTime), sizeof(lingerTime)) == 0);
    }

    return rc;
}

bool Socket::setTcpNoDelay(bool noDelay) {
    bool rc = false;
    int noDelayInt = noDelay ? 1 : 0;
    if (checkSocketHandle()) {
        rc = (setsockopt(_socketHandle, IPPROTO_TCP, TCP_NODELAY,
                         (const void *)(&noDelayInt), sizeof(noDelayInt)) == 0);
    }
    return rc;
}

bool Socket::setTcpQuickAck(bool quickAck) {
  bool rc = false;
  int quickAckInt = quickAck ? 1 : 0;
  if (checkSocketHandle()) {
    rc = (setsockopt(_socketHandle, IPPROTO_TCP, TCP_QUICKACK,
          (const void *)(&quickAckInt), sizeof(quickAckInt)) == 0);
  }
  return rc;
}

/*
 * 是否阻塞
 */
bool Socket::setSoBlocking(bool blockingEnabled) {
    bool rc=false;

    if (checkSocketHandle()) {
        int flags = fcntl(_socketHandle, F_GETFL, NULL);
        if (flags >= 0) {
            if (blockingEnabled) {
                flags &= ~O_NONBLOCK; // clear nonblocking
            } else {
                flags |= O_NONBLOCK;  // set nonblocking
            }

            if (fcntl(_socketHandle, F_SETFL, flags) >= 0) {
                rc = true;
            }
        }
    }

    return rc;
}


/*
 * 得到socket错误
 */
int Socket::getSoError () {
    if (_socketHandle == -1) {
        return EINVAL;
    }

    int lastError = errno;
    int  soError = 0;
    socklen_t soErrorLen = sizeof(soError);
    if (getsockopt(_socketHandle, SOL_SOCKET, SO_ERROR, (void *)(&soError), &soErrorLen) != 0) {
        return lastError;
    }
    if (soErrorLen != sizeof(soError))
        return EINVAL;

    return soError;
}

}
}
