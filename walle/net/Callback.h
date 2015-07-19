#ifndef WALLE_CALLBACK_H_
#define WALLE_CALLBACK_H_
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <walle/sys/wallesys.h>

using namespace walle::sys;
namespace walle {
namespace net{


class Buffer;
class TcpConnection;
typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef boost::function<void()> TimerCallback;
typedef boost::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef boost::function<void (const TcpConnectionPtr&)> CloseCallback;
typedef boost::function<void (const TcpConnectionPtr&)> WriteCompleteCallback;
typedef boost::function<void (const TcpConnectionPtr&, size_t)> HighWaterMarkCallback;

// the data has been read to (buf, len)
typedef boost::function<void (const TcpConnectionPtr&,
                              Buffer*,
                              Time)> MessageCallback;

void defaultConnectionCallback(const TcpConnectionPtr& conn);
void defaultMessageCallback(const TcpConnectionPtr& conn,
                            Buffer* buffer,
                            Time receiveTime);

}
}

#endif
