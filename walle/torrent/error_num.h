#ifndef WALLE_TORRENT_ERROR_NUM_H_
#define WALLE_TORRENT_ERROR_NUM_H_

#include <cerrno>
#include <cstring>


namespace walle {
namespace torrent {
    
class error_number {
public:
  static const int e_access      = EACCES;
  static const int e_again       = EAGAIN;
  static const int e_connreset   = ECONNRESET;
  static const int e_connaborted = ECONNABORTED;
  static const int e_deadlk      = EDEADLK;

  static const int e_noent       = ENOENT;
  static const int e_nodev       = ENODEV;
  static const int e_nomem       = ENOMEM;
  static const int e_notdir      = ENOTDIR;
  static const int e_isdir       = EISDIR;
  
  static const int e_intr        = EINTR;

  error_number() : _errno(0) {}
  error_number(int e) : _errno(e) {}

  bool                is_valid() const 
  { return _errno != 0; }

  int                 value() const 
  { return _errno; }
  const char*         c_str() const  
  { return std::strerror(_errno); }

  bool                is_blocked_momentary() const 
  { return _errno == e_again || _errno == e_intr; }
  bool                is_blocked_prolonged() const 
  { return _errno == e_deadlk; }

  bool                is_closed() const          
 { return _errno == e_connreset || _errno == e_connaborted; }

  bool                is_bad_path() const         
  { return _errno == e_noent || _errno == e_notdir || _errno == e_access; }

  static error_number current()                   
  { return errno; }
  static void         clear_global()              
  { errno = 0; }
  static void         set_global(error_number err) 
  { errno = err._errno; }

  bool operator == (const error_number& e) const   
  { return m_errno == e._errno; }

private:
  int                 _errno;
};

}
}
#endif
