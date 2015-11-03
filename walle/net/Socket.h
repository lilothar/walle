#ifndef WALLE_NET_SOCKET_H_
#define WALLE_NET_SOCKET_H_
#include <string>
#include <walle/sys/Logging.h>
#include <walle/net/Addrinet.h>
struct tcp_info;
namespace walle {
namespace net {

class Socket {

public:
    /*
     * 构造函数
     */
     Socket(bool own = false);

    /*
     * 析构函数
     */
    ~Socket();
    /*
     * 关闭读写
     */
    bool connect( AddrInet& addr) ;
    bool bind( AddrInet& addr) ;
    bool listen(int backlog = 5);

    int accept(AddrInet& peeraddr);

    void shutdown();
    void close();

    /*
     * 把socketHandle,及ipaddress设置到此socket中
     *
     * @param  socketHandle: socket的文件句柄
     * @param hostAddress: 服务器地址
     */

    void setFd(int socketHandle);

    /*
     * 返回文件句柄
     *
     * @return 文件句柄
     */
    int getFd();
    /*
     * 写数据
     */
    int write(const void *data, int len);

    /*
     * 读数据
     */
    int read(void *data, int len);

    /*
     * SetSoKeepAlive
     */
    bool setKeepAlive(bool on) {
        return setIntOption(SO_KEEPALIVE, on ? 1 : 0);
    }

    /*
     * setReuseAddress
     */
    bool setReuseAddress(bool on) {
        return setIntOption(SO_REUSEADDR, on ? 1 : 0);
    }

    /*
     * setSoLinger
     */
    bool setSoLinger (bool doLinger, int seconds);

    /*
     * setTcpNoDelay
     */
    bool setTcpNoDelay(bool noDelay);

    /*
     * setTcpQuickAck
     */
    bool setTcpQuickAck(bool quickAck);

    /*
     * setIntOption
     */
    bool setIntOption(int option, int value);

    /*
     * setTimeOption
     */
    bool setTimeOption(int option, int milliseconds);

    /*
     * 是否阻塞
     */
    bool setSoBlocking(bool on =false);
    /*
     * 检查Socket句柄是否创建
     */
    bool checkSocketHandle();
    bool getTcpInfo(struct tcp_info*) const;
    bool getTcpInfoString(char* buf, int len) const;
    /*
     * 得到Socket错误
     */
    int getSoError();
    int        _savedErr;
protected:
    int        _socketHandle;    // socket文件句柄
    bool       _own;

};
}
}

#endif /*SOCKET_H_*/
