#include <walle/net/Buffer.h>
#include <errno.h>
#include <sys/uio.h>
#include <walle/sys/Types.h>

namespace walle {
namespace net{

const char Buffer::kCRLF[] = "\r\n";

const size_t Buffer::kPrepend;
const size_t Buffer::kInitSize;

ssize_t Buffer::readFd(int fd, int* savedErrno)
{
  char extrabuf[65536];
  struct iovec vec[2];
  const size_t writable = writableBytes();
  vec[0].iov_base = begin()+_writerIndex;
  vec[0].iov_len = writable;
  vec[1].iov_base = extrabuf;
  vec[1].iov_len = sizeof extrabuf;
  const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
  const ssize_t n = ::readv(fd, vec, iovcnt);
  if (n < 0)
  {
    *savedErrno = errno;
  }
  else if (walle::sys::implicit_cast<size_t>(n) <= writable)
  {
    _writerIndex += n;
  }
  else
  {
    _writerIndex = _buffer.size();
    append(extrabuf, n - writable);
  }
  return n;
}
}
}

