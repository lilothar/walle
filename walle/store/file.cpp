#include <walle/store/file.h>
#include <walle/store/myconf.h>

namespace walle {
namespace store {


const int32_t FILEPERM = 00644;          ///< default permission of a new file
const int32_t DIRPERM = 00755;           ///< default permission of a new directory
const int32_t PATHBUFSIZ = 8192;         ///< size of the path buffer
const int32_t IOBUFSIZ = 16384;          ///< size of the IO buffer
const int64_t FILEMAXSIZ = INT64MAX - INT32MAX;  // maximum size of a file
const char* const WALPATHEXT = "wal";    ///< extension of the WAL file
const char WALMAGICDATA[] = "KW\n";      ///< magic data of the WAL file
const uint8_t WALMSGMAGIC = 0xee;        ///< magic data for WAL record


/**
 * File internal.
 */
struct FileCore {
  Mutex alock;                           ///< attribute lock
  TSDKey errmsg;                         ///< error message
  int32_t fd;                            ///< file descriptor
  char* map;                             ///< mapped memory
  int64_t msiz;                          ///< map size
  int64_t lsiz;                          ///< logical size
  int64_t psiz;                          ///< physical size
  std::string path;                      ///< file path
  bool recov;                            ///< flag of recovery
  uint32_t omode;                        ///< open mode
  int32_t walfd;                         ///< file descriptor for WAL
  int64_t walsiz;                        ///< size of WAL
  bool tran;                             ///< whether in transaction
  bool trhard;                           ///< whether hard transaction
  int64_t trbase;                        ///< base offset of guarded region
  int64_t trmsiz;                        ///< minimum size during transaction
};


/**
 * WAL message.
 */
struct WALMessage {
  int64_t off;                           ///< offset of the region
  std::string body;                      ///< body data
};


/**
 * DirStream internal.
 */
struct DirStreamCore {
  Mutex alock;                           ///< attribute lock
  ::DIR* dh;                             ///< directory handle
};


/**
 * Set the error message.
 * @param core the inner condition.
 * @param msg the error message.
 */
static void seterrmsg(FileCore* core, const char* msg);


/**
 * Get the path of the WAL file.
 * @param path the path of the destination file.
 * @return the path of the WAL file.
 */
static std::string walpath(const std::string& path);


/**
 * Write a log message into the WAL file.
 * @param core the inner condition.
 * @param off the offset of the destination.
 * @param size the size of the data region.
 * @param base the base offset.
 * @return true on success, or false on failure.
 */
static bool walwrite(FileCore *core, int64_t off, size_t size, int64_t base);


/**
 * Apply log messages in the WAL file.
 * @param core the inner condition.
 * @return true on success, or false on failure.
 */
static bool walapply(FileCore* core);


/**
 * Write data into a file.
 * @param fd the file descriptor.
 * @param off the offset of the destination.
 * @param buf the pointer to the data region.
 * @param size the size of the data region.
 * @return true on success, or false on failure.
 */
static bool mywrite(int32_t fd, int64_t off, const void* buf, size_t size);


/**
 * Read data from a file.
 * @param fd the file descriptor.
 * @param buf the pointer to the destination region.
 * @param size the size of the data to be read.
 * @return true on success, or false on failure.
 */
static size_t myread(int32_t fd, void* buf, size_t count);


/** Path delimiter character. */
const char File::PATHCHR = MYPATHCHR;


/** Path delimiter string. */
const char* const File::PATHSTR = MYPATHSTR;


/** Extension delimiter character. */
const char File::EXTCHR = MYEXTCHR;


/** Extension delimiter string. */
const char* const File::EXTSTR = MYEXTSTR;


/** Current directory string. */
const char* const File::CDIRSTR = MYCDIRSTR;


/** Parent directory string. */
const char* const File::PDIRSTR = MYPDIRSTR;


/**
 * Default constructor.
 */
File::File() : opq_(NULL) {
  assert(true);
  FileCore* core = new FileCore;
  core->fd = -1;
  core->map = NULL;
  core->msiz = 0;
  core->lsiz = 0;
  core->psiz = 0;
  core->recov = false;
  core->omode = 0;
  core->walfd = -1;
  core->walsiz = 0;
  core->tran = false;
  core->trhard = false;
  core->trmsiz = 0;
  opq_ = core;
}


/**
 * Destructor.
 */
File::~File() {
  assert(true);
  FileCore* core = (FileCore*)opq_;
  if (core->fd >= 0) close();
  delete core;
}


/**
 * Get the last happened error information.
 */
const char* File::error() const {
  assert(true);
  FileCore* core = (FileCore*)opq_;
  const char* msg = (const char*)core->errmsg.get();
  if (!msg) msg = "no error";
  return msg;
}


/**
 * Open a file.
 */
bool File::open(const std::string& path, uint32_t mode, int64_t msiz) {
  assert(msiz >= 0 && msiz <= FILEMAXSIZ);
  FileCore* core = (FileCore*)opq_;
  int32_t oflags = O_RDONLY;
  if (mode & OWRITER) {
    oflags = O_RDWR;
    if (mode & OCREATE) oflags |= O_CREAT;
    if (mode & OTRUNCATE) oflags |= O_TRUNC;
  }
  int32_t fd = ::open(path.c_str(), oflags, FILEPERM);
  if (fd < 0) {
    switch (errno) {
      case EACCES: seterrmsg(core, "open failed (permission denied)"); break;
      case EISDIR: seterrmsg(core, "open failed (directory)"); break;
      case ENOENT: seterrmsg(core, "open failed (file not found)"); break;
      case ENOTDIR: seterrmsg(core, "open failed (invalid path)"); break;
      case ENOSPC: seterrmsg(core, "open failed (no space)"); break;
      default: seterrmsg(core, "open failed"); break;
    }
    return false;
  }
  if (!(mode & ONOLOCK)) {
    struct flock flbuf;
    std::memset(&flbuf, 0, sizeof(flbuf));
    flbuf.l_type = mode & OWRITER ? F_WRLCK : F_RDLCK;
    flbuf.l_whence = SEEK_SET;
    flbuf.l_start = 0;
    flbuf.l_len = 0;
    flbuf.l_pid = 0;
    int32_t cmd = mode & OTRYLOCK ? F_SETLK : F_SETLKW;
    while (::fcntl(fd, cmd, &flbuf) != 0) {
      if (errno != EINTR) {
        seterrmsg(core, "fcntl failed");
        ::close(fd);
        return false;
      }
    }
  }
  struct ::stat sbuf;
  if (::fstat(fd, &sbuf) != 0) {
    seterrmsg(core, "fstat failed");
    ::close(fd);
    return false;
  }
  if (!S_ISREG(sbuf.st_mode)) {
    seterrmsg(core, "not a regular file");
    ::close(fd);
    return false;
  }
  bool recov = false;
  if ((!(mode & OWRITER) || !(mode & OTRUNCATE)) && !(mode & ONOLOCK)) {
    const std::string& wpath = walpath(path);
    int32_t walfd = ::open(wpath.c_str(), O_RDWR, FILEPERM);
    if (walfd >= 0) {
      struct ::stat wsbuf;
      if (::fstat(walfd, &wsbuf) == 0 && wsbuf.st_uid == sbuf.st_uid) {
        recov = true;
        if (wsbuf.st_size >= (int64_t)sizeof(WALMAGICDATA)) {
          char mbuf[sizeof(WALMAGICDATA)];
          if (myread(walfd, mbuf, sizeof(mbuf)) &&
              !std::memcmp(mbuf, WALMAGICDATA, sizeof(WALMAGICDATA))) {
            int32_t ofd = mode & OWRITER ? fd : ::open(path.c_str(), O_WRONLY, FILEPERM);
            if (ofd >= 0) {
              core->fd = ofd;
              core->walfd = walfd;
              walapply(core);
              if (ofd != fd && ::close(ofd) != 0) seterrmsg(core, "close failed");
              if (::ftruncate(walfd, 0) != 0) seterrmsg(core, "ftruncate failed");
              core->fd = -1;
              core->walfd = -1;
              if (::fstat(fd, &sbuf) != 0) {
                seterrmsg(core, "fstat failed");
                ::close(fd);
                return false;
              }
            } else {
              seterrmsg(core, "open failed");
            }
          }
        }
      }
      if (::close(walfd) != 0) seterrmsg(core, "close failed");
      if (::unlink(wpath.c_str()) != 0) seterrmsg(core, "unlink failed");
    }
  }
  int64_t lsiz = sbuf.st_size;
  int64_t psiz = lsiz;
  int64_t diff = msiz % PAGESIZ;
  if (diff > 0) msiz += PAGESIZ - diff;
  int32_t mprot = PROT_READ;
  if (mode & OWRITER) {
    mprot |= PROT_WRITE;
  } else if (msiz > lsiz) {
    msiz = lsiz;
  }
  void* map = NULL;
  if (msiz > 0) {
    map = ::mmap(0, msiz, mprot, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
      seterrmsg(core, "mmap failed");
      ::close(fd);
      return false;
    }
  }
  core->fd = fd;
  core->map = (char*)map;
  core->msiz = msiz;
  core->lsiz = lsiz;
  core->psiz = psiz;
  core->recov = recov;
  core->omode = mode;
  core->path.append(path);
  return true;
}


/**
 * Close the file.
 */
bool File::close() {
  assert(true);
  FileCore* core = (FileCore*)opq_;
  bool err = false;
  if (core->tran && !end_transaction(false)) err = true;
  if (core->walfd >= 0) {
    if (::close(core->walfd) != 0) {
      seterrmsg(core, "close failed");
      err = true;
    }
    const std::string& wpath = walpath(core->path);
    struct ::stat sbuf;
    if (::lstat(wpath.c_str(), &sbuf) == 0 && S_ISREG(sbuf.st_mode) &&
        ::unlink(wpath.c_str()) != 0) {
      seterrmsg(core, "unlink failed");
      err = true;
    }
  }
  if (core->msiz > 0 && ::munmap(core->map, core->msiz) != 0) {
    seterrmsg(core, "munmap failed");
    err = true;
  }
  if (core->psiz != core->lsiz && ::ftruncate(core->fd, core->lsiz) != 0) {
    seterrmsg(core, "ftruncate failed");
    err = true;
  }
  if (!(core->omode & ONOLOCK)) {
    struct flock flbuf;
    std::memset(&flbuf, 0, sizeof(flbuf));
    flbuf.l_type = F_UNLCK;
    flbuf.l_whence = SEEK_SET;
    flbuf.l_start = 0;
    flbuf.l_len = 0;
    flbuf.l_pid = 0;
    while (::fcntl(core->fd, F_SETLKW, &flbuf) != 0) {
      if (errno != EINTR) {
        seterrmsg(core, "fcntl failed");
        err = true;
        break;
      }
    }
  }
  if (::close(core->fd) != 0) {
    seterrmsg(core, "close failed");
    err = true;
  }
  core->fd = -1;
  core->map = NULL;
  core->msiz = 0;
  core->lsiz = 0;
  core->psiz = 0;
  core->path.clear();
  core->walfd = -1;
  core->walsiz = 0;
  core->tran = false;
  core->trhard = false;
  core->trmsiz = 0;
  return !err;
}


/**
 * Write data.
 */
bool File::write(int64_t off, const void* buf, size_t size) {
  assert(off >= 0 && off <= FILEMAXSIZ && buf && size <= MEMMAXSIZ);
  if (size < 1) return true;
  FileCore* core = (FileCore*)opq_;
  if (core->tran && !walwrite(core, off, size, core->trbase)) return false;
  int64_t end = off + size;
  core->alock.lock();
  if (end <= core->msiz) {
    if (end > core->psiz) {
      int64_t psiz = end + core->psiz / 2;
      int64_t diff = psiz % PAGESIZ;
      if (diff > 0) psiz += PAGESIZ - diff;
      if (psiz > core->msiz) psiz = core->msiz;
      if (::ftruncate(core->fd, psiz) != 0) {
        seterrmsg(core, "ftruncate failed");
        core->alock.unlock();
        return false;
      }
      core->psiz = psiz;
    }
    if (end > core->lsiz) core->lsiz = end;
    core->alock.unlock();
    std::memcpy(core->map + off, buf, size);
    return true;
  }
  if (off < core->msiz) {
    if (end > core->psiz) {
      if (::ftruncate(core->fd, end) != 0) {
        seterrmsg(core, "ftruncate failed");
        core->alock.unlock();
        return false;
      }
      core->psiz = end;
    }
    size_t hsiz = core->msiz - off;
    std::memcpy(core->map + off, buf, hsiz);
    off += hsiz;
    buf = (char*)buf + hsiz;
    size -= hsiz;
  }
  if (end > core->lsiz) core->lsiz = end;
  if (end > core->psiz) {
    if (core->psiz < core->msiz && ::ftruncate(core->fd, core->msiz) != 0) {
      seterrmsg(core, "ftruncate failed");
      core->alock.unlock();
      return false;
    }
    core->psiz = end;
  }
  core->alock.unlock();
  if (!mywrite(core->fd, off, buf, size)) {
    seterrmsg(core, "mywrite failed");
    return false;
  }
  return true;
}


/**
 * Write data with assuring the region does not spill from the file size.
 */
bool File::write_fast(int64_t off, const void* buf, size_t size) {
  assert(off >= 0 && off <= FILEMAXSIZ && buf && size <= MEMMAXSIZ);
  FileCore* core = (FileCore*)opq_;
  if (core->tran && !walwrite(core, off, size, core->trbase)) return false;
  int64_t end = off + size;
  if (end <= core->msiz) {
    std::memcpy(core->map + off, buf, size);
    return true;
  }
  if (off < core->msiz) {
    size_t hsiz = core->msiz - off;
    std::memcpy(core->map + off, buf, hsiz);
    off += hsiz;
    buf = (char*)buf + hsiz;
    size -= hsiz;
  }
  if (!mywrite(core->fd, off, buf, size)) {
    seterrmsg(core, "mywrite failed");
    return false;
  }
  return true;
}


/**
 * Write data at the end of the file.
 */
bool File::append(const void* buf, size_t size) {
  assert(buf && size <= MEMMAXSIZ);
  if (size < 1) return true;
  FileCore* core = (FileCore*)opq_;
  core->alock.lock();
  int64_t off = core->lsiz;
  int64_t end = off + size;
  if (end <= core->msiz) {
    if (end > core->psiz) {
      int64_t psiz = end + core->psiz / 2;
      int64_t diff = psiz % PAGESIZ;
      if (diff > 0) psiz += PAGESIZ - diff;
      if (psiz > core->msiz) psiz = core->msiz;
      if (::ftruncate(core->fd, psiz) != 0) {
        seterrmsg(core, "ftruncate failed");
        core->alock.unlock();
        return false;
      }
      core->psiz = psiz;
    }
    core->lsiz = end;
    core->alock.unlock();
    std::memcpy(core->map + off, buf, size);
    return true;
  }
  if (off < core->msiz) {
    if (end > core->psiz) {
      if (::ftruncate(core->fd, end) != 0) {
        seterrmsg(core, "ftruncate failed");
        core->alock.unlock();
        return false;
      }
      core->psiz = end;
    }
    size_t hsiz = core->msiz - off;
    std::memcpy(core->map + off, buf, hsiz);
    off += hsiz;
    buf = (char*)buf + hsiz;
    size -= hsiz;
  }
  core->lsiz = end;
  core->psiz = end;
  core->alock.unlock();
  while (true) {
    ssize_t wb = ::pwrite(core->fd, buf, size, off);
    if (wb >= (ssize_t)size) {
      return true;
    } else if (wb > 0) {
      buf = (char*)buf + wb;
      size -= wb;
      off += wb;
    } else if (wb == -1) {
      if (errno != EINTR) {
        seterrmsg(core, "pwrite failed");
        return false;
      }
    } else if (size > 0) {
      seterrmsg(core, "pwrite failed");
      return false;
    }
  }
  return true;
}


/**
 * Read data.
 */
bool File::read(int64_t off, void* buf, size_t size) {
  assert(off >= 0 && off <= FILEMAXSIZ && buf && size <= MEMMAXSIZ);
  if (size < 1) return true;
  FileCore* core = (FileCore*)opq_;
  int64_t end = off + size;
  core->alock.lock();
  if (end > core->lsiz) {
    seterrmsg(core, "out of bounds");
    core->alock.unlock();
    return false;
  }
  core->alock.unlock();
  if (end <= core->msiz) {
    std::memcpy(buf, core->map + off, size);
    return true;
  }
  if (off < core->msiz) {
    int64_t hsiz = core->msiz - off;
    std::memcpy(buf, core->map + off, hsiz);
    off += hsiz;
    buf = (char*)buf + hsiz;
    size -= hsiz;
  }
  while (true) {
    ssize_t rb = ::pread(core->fd, buf, size, off);
    if (rb >= (ssize_t)size) {
      break;
    } else if (rb > 0) {
      buf = (char*)buf + rb;
      size -= rb;
      off += rb;
    } else if (rb == -1) {
      if (errno != EINTR) {
        seterrmsg(core, "pread failed");
        return false;
      }
    } else if (size > 0) {
      Thread::yield();
    }
  }
  return true;
}


/**
 * Read data with assuring the region does not spill from the file size.
 */
bool File::read_fast(int64_t off, void* buf, size_t size) {
  assert(off >= 0 && off <= FILEMAXSIZ && buf && size <= MEMMAXSIZ);
  FileCore* core = (FileCore*)opq_;
  int64_t end = off + size;
  if (end <= core->msiz) {
    std::memcpy(buf, core->map + off, size);
    return true;
  }
  if (off < core->msiz) {
    int64_t hsiz = core->msiz - off;
    std::memcpy(buf, core->map + off, hsiz);
    off += hsiz;
    buf = (char*)buf + hsiz;
    size -= hsiz;
  }
  while (true) {
    ssize_t rb = ::pread(core->fd, buf, size, off);
    if (rb >= (ssize_t)size) {
      break;
    } else if (rb > 0) {
      buf = (char*)buf + rb;
      size -= rb;
      off += rb;
      Thread::yield();
    } else if (rb == -1) {
      if (errno != EINTR) {
        seterrmsg(core, "pread failed");
        return false;
      }
    } else if (size > 0) {
      if (end > core->lsiz) {
        seterrmsg(core, "out of bounds");
        return false;
      }
      Thread::yield();
    }
  }
  return true;
}


/**
 * Truncate the file.
 */
bool File::truncate(int64_t size) {
  assert(size >= 0 && size <= FILEMAXSIZ);
  FileCore* core = (FileCore*)opq_;
  if (core->tran && size < core->trmsiz) {
    if (!walwrite(core, size, core->trmsiz - size, core->trbase)) return false;
    core->trmsiz = size;
  }
  bool err = false;
  core->alock.lock();
  if (::ftruncate(core->fd, size) != 0) {
    seterrmsg(core, "ftruncate failed");
    err = true;
  }
  core->lsiz = size;
  core->psiz = size;
  core->alock.unlock();
  return !err;
}


/**
 * Synchronize updated contents with the file and the device.
 */
bool File::synchronize(bool hard) {
  assert(true);
  FileCore* core = (FileCore*)opq_;
  bool err = false;
  core->alock.lock();
  if (hard && core->msiz > 0) {
    int64_t msiz = core->msiz;
    if (msiz > core->psiz) msiz = core->psiz;
    if (msiz > 0 && ::msync(core->map, msiz, MS_SYNC) != 0) {
      seterrmsg(core, "msync failed");
      err = true;
    }
  }
  if (::ftruncate(core->fd, core->lsiz) != 0) {
    seterrmsg(core, "ftruncate failed");
    err = true;
  }
  if (core->psiz > core->lsiz) core->psiz = core->lsiz;
  if (hard && ::fsync(core->fd) != 0) {
    seterrmsg(core, "fsync failed");
    err = true;
  }
  core->alock.unlock();
  return !err;
}


/**
 * Refresh the internal state for update by others.
 */
bool File::refresh() {
  assert(true);
  FileCore* core = (FileCore*)opq_;
  struct ::stat sbuf;
  if (::fstat(core->fd, &sbuf) != 0) {
    seterrmsg(core, "fstat failed");
    return false;
  }
  core->lsiz = sbuf.st_size;
  core->psiz = sbuf.st_size;
  bool err = false;
  int64_t msiz = core->msiz;
  if (msiz > core->psiz) msiz = core->psiz;
  if (msiz > 0 && ::msync(core->map, msiz, MS_INVALIDATE) != 0) {
    seterrmsg(core, "msync failed");
    err = true;
  }
  return !err;
}


/**
 * Begin transaction.
 */
bool File::begin_transaction(bool hard, int64_t off) {
  assert(off >= 0 && off <= FILEMAXSIZ);
  FileCore* core = (FileCore*)opq_;
  core->alock.lock();
  if (core->walfd < 0) {
    const std::string& wpath = walpath(core->path);
    int32_t fd = ::open(wpath.c_str(), O_RDWR | O_CREAT | O_TRUNC, FILEPERM);
    if (fd < 0) {
      switch (errno) {
        case EACCES: seterrmsg(core, "open failed (permission denied)"); break;
        case ENOENT: seterrmsg(core, "open failed (file not found)"); break;
        case ENOTDIR: seterrmsg(core, "open failed (invalid path)"); break;
        default: seterrmsg(core, "open failed"); break;
      }
      core->alock.unlock();
      return false;
    }
    core->walfd = fd;
  }
  char wbuf[NUMBUFSIZ];
  char* wp = wbuf;
  std::memcpy(wp, WALMAGICDATA, sizeof(WALMAGICDATA));
  wp += sizeof(WALMAGICDATA);
  int64_t num = hton64(core->lsiz);
  std::memcpy(wp, &num, sizeof(num));
  wp += sizeof(num);
  int64_t wsiz = wp - wbuf;
  if (!mywrite(core->walfd, 0, wbuf, wsiz)) {
    seterrmsg(core, "mywrite failed");
    core->alock.unlock();
    return false;
  }
  core->walsiz = wsiz;
  core->tran = true;
  core->trhard = hard;
  core->trbase = off;
  core->trmsiz = core->lsiz;
  core->alock.unlock();
  return true;
}


/**
 * Commit transaction.
 */
bool File::end_transaction(bool commit) {
  assert(true);
  FileCore* core = (FileCore*)opq_;
  bool err = false;
  core->alock.lock();
  if (!commit && !walapply(core)) err = true;
  if (!err) {
    if (core->walsiz <= IOBUFSIZ) {
      char mbuf[IOBUFSIZ];
      std::memset(mbuf, 0, core->walsiz);
      if (!mywrite(core->walfd, 0, mbuf, core->walsiz)) {
        seterrmsg(core, "mywrite failed");
        err = true;
      }
    } else {
      if (::ftruncate(core->walfd, 0) != 0) {
        seterrmsg(core, "ftruncate failed");
        err = true;
      }
    }
  }
  if (core->trhard) {
    int64_t msiz = core->msiz;
    if (msiz > core->psiz) msiz = core->psiz;
    if (msiz > 0 && ::msync(core->map, msiz, MS_SYNC) != 0) {
      seterrmsg(core, "msync failed");
      err = true;
    }
    if (::fsync(core->fd) != 0) {
      seterrmsg(core, "fsync failed");
      err = true;
    }
    if (::fsync(core->walfd) != 0) {
      seterrmsg(core, "fsync failed");
      err = true;
    }
  }
  core->tran = false;
  core->alock.unlock();
  return !err;
}


/**
 * Write a WAL message of transaction explicitly.
 */
bool File::write_transaction(int64_t off, size_t size) {
  assert(off >= 0 && off <= FILEMAXSIZ && size <= MEMMAXSIZ);
  FileCore* core = (FileCore*)opq_;
  return walwrite(core, off, size, 0);
}


/**
 * Get the size of the file.
 */
int64_t File::size() const {
  assert(true);
  FileCore* core = (FileCore*)opq_;
  return core->lsiz;
}


/**
 * Get the path of the file.
 */
std::string File::path() const {
  assert(true);
  FileCore* core = (FileCore*)opq_;
  return core->path;
}


/**
 * Check whether the file was recovered or not.
 */
bool File::recovered() const {
  assert(true);
  FileCore* core = (FileCore*)opq_;
  return core->recov;
}


/**
 * Read the whole data from a file.
 */
char* File::read_file(const std::string& path, int64_t* sp, int64_t limit) {
  assert(sp);
  if (limit < 0) limit = INT64MAX;
  int32_t fd = ::open(path.c_str(), O_RDONLY, FILEPERM);
  if (fd < 0) return NULL;
  struct stat sbuf;
  if (::fstat(fd, &sbuf) == -1 || !S_ISREG(sbuf.st_mode)) {
    ::close(fd);
    return NULL;
  }
  if (limit > (int64_t)sbuf.st_size) limit = sbuf.st_size;
  char* buf = new char[limit+1];
  char* wp = buf;
  ssize_t rsiz;
  while ((rsiz = ::read(fd, wp, limit - (wp - buf))) > 0) {
    wp += rsiz;
  }
  *wp = '\0';
  ::close(fd);
  *sp = wp - buf;
  return buf;
}


/**
 * Write the whole data into a file.
 */
bool File::write_file(const std::string& path, const char* buf, int64_t size) {
  assert(buf && size >= 0 && size <= FILEMAXSIZ);
  int32_t fd = ::open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, FILEPERM);
  if (fd < 0) return false;
  bool err = false;
  const char* rp = buf;
  while (!err && size > 0) {
    ssize_t wb = ::write(fd, rp, size);
    switch (wb) {
      case -1: {
        if (errno != EINTR) {
          err = true;
          break;
        }
      }
      case 0: {
        break;
      }
      default: {
        rp += wb;
        size -= wb;
        break;
      }
    }
  }
  if (::close(fd) != 0) err = true;
  return !err;
}


/**
 * Get the status information of a file.
 */
bool File::status(const std::string& path, Status* buf) {
  assert(true);
  struct ::stat sbuf;
  if (::lstat(path.c_str(), &sbuf) != 0) return false;
  if (buf) {
    buf->isdir = S_ISDIR(sbuf.st_mode);
    buf->size = sbuf.st_size;
    buf->mtime = sbuf.st_mtime;
  }
  return true;
}


/**
 * Get the absolute path of a file.
 */
std::string File::absolute_path(const std::string& path) {
  assert(true);
  char buf[PATHBUFSIZ];
  if (!realpath(path.c_str(), buf)) return "";
  return std::string(buf);
}


/**
 * Remove a file.
 */
bool File::remove(const std::string& path) {
  assert(true);
  return ::unlink(path.c_str()) == 0;
}


/**
 * Change the name or location of a file.
 */
bool File::rename(const std::string& opath, const std::string& npath) {
  assert(true);
  return ::rename(opath.c_str(), npath.c_str()) == 0;
}


/**
 * Read a directory.
 */
bool File::read_directory(const std::string& path, std::vector<std::string>* strvec) {
  assert(strvec);
  ::DIR* dir = ::opendir(path.c_str());
  if (!dir) return false;
  struct ::dirent *dp;
  while ((dp = ::readdir(dir)) != NULL) {
    if (std::strcmp(dp->d_name, CDIRSTR) && std::strcmp(dp->d_name, PDIRSTR))
      strvec->push_back(dp->d_name);
  }
  if (::closedir(dir) != 0) return false;
  return true;
}


/**
 * Make a directory.
 */
bool File::make_directory(const std::string& path) {
  assert(true);
  return ::mkdir(path.c_str(), DIRPERM) == 0;
}


/**
 * Remove a directory.
 */
bool File::remove_directory(const std::string& path) {
  assert(true);
  return ::rmdir(path.c_str()) == 0;
}


/**
 * Remove a file or a directory recursively.
 */
bool File::remove_recursively(const std::string& path) {
  bool err = false;
  std::vector<std::string> list;
  list.push_back(path);
  while (!list.empty()) {
    const std::string& cpath = list.back();
    Status sbuf;
    if (status(cpath, &sbuf)) {
      if (sbuf.isdir) {
        if (remove_directory(cpath)) {
          list.pop_back();
        } else {
          DirStream dir;
          if (dir.open(cpath)) {
            std::string ccname;
            while (dir.read(&ccname)) {
              const std::string& ccpath = cpath + MYPATHCHR + ccname;
              if (!remove(ccpath)) list.push_back(ccpath);
            }
            if (!dir.close()) err = true;
          } else {
            list.pop_back();
            err = true;
          }
        }
      } else {
        if (!remove(cpath)) err = true;
        list.pop_back();
      }
    } else {
      list.pop_back();
      err = true;
    }
  }
  return !err;
}


/**
 * Get the path of the current working directory.
 */
std::string File::get_current_directory() {
  assert(true);
  char buf[PATHBUFSIZ];
  if (!::getcwd(buf, sizeof(buf))) return "";
  return std::string(buf);
}


/**
 * Set the current working directory.
 */
bool File::set_current_directory(const std::string& path) {
  assert(true);
  return ::chdir(path.c_str()) == 0;
}


/**
 * Synchronize the whole of the file system with the device.
 */
bool File::synchronize_whole() {
  assert(true);
  ::sync();
  return true;
}



/**
 * Default constructor.
 */
DirStream::DirStream() : opq_(NULL) {
  assert(true);
  DirStreamCore* core = new DirStreamCore;
  core->dh = NULL;
  opq_ = core;
}


/**
 * Destructor.
 */
DirStream::~DirStream() {
  assert(true);
  DirStreamCore* core = (DirStreamCore*)opq_;
  if (core->dh) close();
  delete core;
}


/**
 * Open a directory.
 */
bool DirStream::open(const std::string& path) {
  assert(true);
  DirStreamCore* core = (DirStreamCore*)opq_;
  ScopedMutex lock(&core->alock);
  if (core->dh) return false;
  ::DIR* dh = ::opendir(path.c_str());
  if (!dh) return false;
  core->dh = dh;
  return true;
}


/**
 * Close the file.
 */
bool DirStream::close() {
  assert(true);
  DirStreamCore* core = (DirStreamCore*)opq_;
  ScopedMutex lock(&core->alock);
  if (!core->dh) return false;
  bool err = false;
  if (::closedir(core->dh) != 0) err = true;
  core->dh = NULL;
  return !err;
}


/**
 * Read the next file in the directory.
 */
bool DirStream::read(std::string* path) {
  assert(path);
  DirStreamCore* core = (DirStreamCore*)opq_;
  ScopedMutex lock(&core->alock);
  if (!core->dh) return false;
  struct ::dirent *dp;
  do {
    dp = ::readdir(core->dh);
    if (!dp) return false;
  } while (!std::strcmp(dp->d_name, File::CDIRSTR) || !std::strcmp(dp->d_name, File::PDIRSTR));
  path->clear();
  path->append(dp->d_name);
  return true;
}


/**
 * Set the error message.
 */
static void seterrmsg(FileCore* core, const char* msg) {
  assert(core && msg);
  core->errmsg.set((void*)msg);
}


/**
 * Get the path of the WAL file.
 */
static std::string walpath(const std::string& path) {
  assert(true);
  return path + File::EXTCHR + WALPATHEXT;
}


/**
 * Write a log message into the WAL file.
 */
static bool walwrite(FileCore *core, int64_t off, size_t size, int64_t base) {
  assert(core && off >= 0 && off <= FILEMAXSIZ && size <= MEMMAXSIZ && base >= 0);
  bool err = false;
  if (off < base) {
    int64_t diff = base - off;
    if (diff >= (int64_t)size) return true;
    off = base;
    size -= diff;
  }
  int64_t rem = core->trmsiz - off;
  if (rem < 1) return true;
  if (rem < (int64_t)size) size = rem;
  char stack[IOBUFSIZ];
  size_t rsiz = sizeof(int8_t) + sizeof(int64_t) * 2 + size;
  char* rbuf = rsiz > sizeof(stack) ? new char[rsiz] : stack;
  char* wp = rbuf;
  *(wp++) = WALMSGMAGIC;
  int64_t num = hton64(off);
  std::memcpy(wp, &num, sizeof(num));
  wp += sizeof(num);
  num = hton64(size);
  std::memcpy(wp, &num, sizeof(num));
  wp += sizeof(num);
  core->alock.lock();
  int64_t end = off + size;
  if (end <= core->msiz) {
    std::memcpy(wp, core->map + off, size);
  } else {
    if (off < core->msiz) {
      int64_t hsiz = core->msiz - off;
      std::memcpy(wp, core->map + off, hsiz);
      off += hsiz;
      wp += hsiz;
      size -= hsiz;
    }
    while (true) {
      ssize_t rb = ::pread(core->fd, wp, size, off);
      if (rb >= (ssize_t)size) {
        break;
      } else if (rb > 0) {
        wp += rb;
        size -= rb;
        off += rb;
      } else if (rb == -1) {
        if (errno != EINTR) {
          err = true;
          break;
        }
      } else {
        err = true;
        break;
      }
    }
    if (err) {
      seterrmsg(core, "pread failed");
      std::memset(wp, 0, size);
    }
  }
  if (!mywrite(core->walfd, core->walsiz, rbuf, rsiz)) {
    seterrmsg(core, "mywrite failed");
    err = true;
  }
  if (core->trhard && ::fsync(core->walfd) != 0) {
    seterrmsg(core, "fsync failed");
    err = true;
  }
  core->walsiz += rsiz;
  if (rbuf != stack) delete[] rbuf;
  core->alock.unlock();
  return !err;
}


/**
 * Apply log messages in the WAL file.
 */
static bool walapply(FileCore* core) {
  assert(core);
  bool err = false;
  char buf[IOBUFSIZ];
  int64_t hsiz = sizeof(WALMAGICDATA) + sizeof(int64_t);
  int64_t rem = ::lseek(core->walfd, 0, SEEK_END);
  if (rem < hsiz) {
    seterrmsg(core, "lseek failed");
    return false;
  }
  if (::lseek(core->walfd, 0, SEEK_SET) != 0) {
    seterrmsg(core, "lseek failed");
    return false;
  }
  if (!myread(core->walfd, buf, hsiz)) {
    seterrmsg(core, "myread failed");
    return false;
  }
  if (*buf == 0) return true;
  if (std::memcmp(buf, WALMAGICDATA, sizeof(WALMAGICDATA))) {
    seterrmsg(core, "invalid magic data of WAL");
    return false;
  }
  int64_t osiz;
  std::memcpy(&osiz, buf + sizeof(WALMAGICDATA), sizeof(osiz));
  osiz = ntoh64(osiz);
  rem -= hsiz;
  hsiz = sizeof(uint8_t) + sizeof(int64_t) * 2;
  std::vector<WALMessage> msgs;
  int64_t end = 0;
  while (rem >= hsiz) {
    if (!myread(core->walfd, buf, hsiz)) {
      seterrmsg(core, "myread failed");
      err = true;
      break;
    }
    if (*buf == 0) {
      rem = 0;
      break;
    }
    rem -= hsiz;
    char* rp = buf;
    if (*(uint8_t*)(rp++) != WALMSGMAGIC) {
      seterrmsg(core, "invalid magic data of WAL message");
      err = true;
      break;
    }
    if (rem > 0) {
      int64_t off;
      std::memcpy(&off, rp, sizeof(off));
      off = ntoh64(off);
      rp += sizeof(off);
      int64_t size;
      std::memcpy(&size, rp, sizeof(size));
      size = ntoh64(size);
      rp += sizeof(size);
      if (off < 0 || size < 0) {
        seterrmsg(core, "invalid meta data of WAL message");
        err = true;
        break;
      }
      if (rem < size) {
        seterrmsg(core, "too short WAL message");
        err = true;
        break;
      }
      char* rbuf = size > (int64_t)sizeof(buf) ? new char[size] : buf;
      if (!myread(core->walfd, rbuf, size)) {
        seterrmsg(core, "myread failed");
        if (rbuf != buf) delete[] rbuf;
        err = true;
        break;
      }
      rem -= size;
      WALMessage msg = { off, std::string(rbuf, size) };
      msgs.push_back(msg);
      if (off + size > end) end = off + size;
      if (rbuf != buf) delete[] rbuf;
    }
  }
  if (rem != 0) {
    if (!myread(core->walfd, buf, 1)) {
      seterrmsg(core, "myread failed");
      err = true;
    } else if (*buf != 0) {
      seterrmsg(core, "too few messages of WAL");
      err = true;
    }
  }
  if (end > core->msiz) end = core->msiz;
  if (core->psiz < end && ::ftruncate(core->fd, end) != 0) {
    seterrmsg(core, "ftruncate failed");
    err = true;
  }
  for (int64_t i = (int64_t)msgs.size() - 1; i >= 0; i--) {
    const WALMessage& msg = msgs[i];
    int64_t off = msg.off;
    const char* rbuf = msg.body.c_str();
    size_t size = msg.body.size();
    int64_t end = off + size;
    if (end <= core->msiz) {
      std::memcpy(core->map + off, rbuf, size);
    } else {
      if (off < core->msiz) {
        size_t hsiz = core->msiz - off;
        std::memcpy(core->map + off, rbuf, hsiz);
        off += hsiz;
        rbuf += hsiz;
        size -= hsiz;
      }
      while (true) {
        ssize_t wb = ::pwrite(core->fd, rbuf, size, off);
        if (wb >= (ssize_t)size) {
          break;
        } else if (wb > 0) {
          rbuf += wb;
          size -= wb;
          off += wb;
        } else if (wb == -1) {
          if (errno != EINTR) {
            seterrmsg(core, "pwrite failed");
            err = true;
            break;
          }
        } else if (size > 0) {
          seterrmsg(core, "pwrite failed");
          err = true;
          break;
        }
      }
    }
  }
  if (::ftruncate(core->fd, osiz) == 0) {
    core->lsiz = osiz;
    core->psiz = osiz;
  } else {
    seterrmsg(core, "ftruncate failed");
    err = true;
  }
  return !err;
}


static bool mywrite(int32_t fd, int64_t off, const void* buf, size_t size) {
  assert(fd >= 0 && off >= 0 && off <= FILEMAXSIZ && buf && size <= MEMMAXSIZ);
  while (true) {
    ssize_t wb = ::pwrite(fd, buf, size, off);
    if (wb >= (ssize_t)size) {
      return true;
    } else if (wb > 0) {
      buf = (char*)buf + wb;
      size -= wb;
      off += wb;
    } else if (wb == -1) {
      if (errno != EINTR) return false;
    } else if (size > 0) {
      return false;
    }
  }
  return true;
}


/**
 * Read data from a file.
 */

static size_t myread(int32_t fd, void* buf, size_t size) {
  assert(fd >= 0 && buf && size <= MEMMAXSIZ);
  while (true) {
    ssize_t rb = ::read(fd, buf, size);
    if (rb >= (ssize_t)size) {
      break;
    } else if (rb > 0) {
      buf = (char*)buf + rb;
      size -= rb;
    } else if (rb == -1) {
      if (errno != EINTR) return false;
    } else if (size > 0) {
      return false;
    }
  }
  return true;
}




}
}                                        // common namespace

// END OF FILE
