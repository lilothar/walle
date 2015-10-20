#ifndef WALLE_TORRENT_EXCEPTIONS_H_
#define WALLE_TORRENT_EXCEPTIONS_H_

#include <exception>
#include <string>

namespace torrent {

class  base_error : public std::exception {
public:
  virtual ~base_error() throw() {}
};

class  internal_error : public base_error {
public:
  internal_error(const char* msg)        { initialize(msg); }
  internal_error(const std::string& msg) { initialize(msg); }
  virtual ~internal_error() throw() {}

  virtual const char* what() const throw() { return _msg.c_str(); }
  const std::string&  backtrace() const throw() { return _backtrace; }

private:

  void initialize(const std::string& msg);

  std::string _msg;
  std::string _backtrace;
};


class  network_error : public base_error {
public:
  virtual ~network_error() throw() {}
};

class LIBTORRENT_EXPORT communication_error : public network_error {
public:
  communication_error(const char* msg)        { initialize(msg); }
  communication_error(const std::string& msg) { initialize(msg); }
  virtual ~communication_error() throw() {}

  virtual const char* what() const throw() { return m_msg.c_str(); }

private:
  // Use this function for breaking on throws.
  void initialize(const std::string& msg);

  std::string m_msg;
};

class LIBTORRENT_EXPORT connection_error : public network_error {
public:
  connection_error(int err) : m_errno(err) {}
  virtual ~connection_error() throw() {}

  virtual const char* what() const throw();

  int get_errno() const { return m_errno; }

private:
  int m_errno;
};

class LIBTORRENT_EXPORT address_info_error : public network_error {
public:
  address_info_error(int err) : m_errno(err) {}
  virtual ~address_info_error() throw() {}

  virtual const char* what() const throw();

  int get_errno() const { return m_errno; }

private:
  int m_errno;
};

class LIBTORRENT_EXPORT close_connection : public network_error {
public:
  virtual ~close_connection() throw() {}
};

class LIBTORRENT_EXPORT blocked_connection : public network_error {
public:
  virtual ~blocked_connection() throw() {}
};

// Stuff like bad torrent file, disk space and permissions.
class LIBTORRENT_EXPORT local_error : public base_error {
public:
  virtual ~local_error() throw() {}
};

class LIBTORRENT_EXPORT storage_error : public local_error {
public:
  storage_error(const char* msg)       { initialize(msg); }
  storage_error(const std::string& msg) { initialize(msg); }
  virtual ~storage_error() throw() {}

  virtual const char* what() const throw() { return m_msg.c_str(); }

private:
  // Use this function for breaking on throws.
  void initialize(const std::string& msg);

  std::string m_msg;
};

class LIBTORRENT_EXPORT resource_error : public local_error {
public:
  resource_error(const char* msg) { initialize(msg); }
  resource_error(const std::string& msg) { initialize(msg); }
  virtual ~resource_error() throw() {}

  virtual const char* what() const throw() { return m_msg.c_str(); }

private:
  // Use this function for breaking on throws.
  void initialize(const std::string& msg);

  std::string m_msg;
};

class LIBTORRENT_EXPORT input_error : public local_error {
public:
  input_error(const char* msg) { initialize(msg); }
  input_error(const std::string& msg) { initialize(msg); }
  virtual ~input_error() throw() {}

  virtual const char* what() const throw() { return m_msg.c_str(); }

private:
  // Use this function for breaking on throws.
  void initialize(const std::string& msg);

  std::string m_msg;
};

class LIBTORRENT_EXPORT bencode_error : public input_error {
public:
  bencode_error(const char* msg) : input_error(msg) {}
  bencode_error(const std::string& msg) : input_error(msg) {}

  virtual ~bencode_error() throw() {}
};

class LIBTORRENT_EXPORT shutdown_exception : public base_error {
public:
  virtual ~shutdown_exception() throw() {}
};

}

#endif
