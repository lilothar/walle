#ifndef WALLE_CALLBACK_H_
#define WALLE_CALLBACK_H_
#include <walle/algo/functional.h>
#include <walle/algo/memory.h>
#include <walle/sys/wallesys.h>

using namespace walle::sys;
namespace walle {
namespace net{


class Buffer;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void()> TimerCallback;
typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr&)> CloseCallback;
typedef std::function<void (const TcpConnectionPtr&)> WriteCompleteCallback;
typedef std::function<void (const TcpConnectionPtr&, size_t)> HighWaterMarkCallback;

// the data has been read to (buf, len)
typedef std::function<void (const TcpConnectionPtr&,
                              Buffer*,
                              Time)> MessageCallback;

void defaultConnectionCallback(const TcpConnectionPtr& conn);
void defaultMessageCallback(const TcpConnectionPtr& conn,
                            Buffer* buffer,
                            Time receiveTime);

}
}

#endif
