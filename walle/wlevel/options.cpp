#include <walle/wlevel/options.h>

#include <walle/wlevel/comparator.h>
#include <walle/wlevel/env.h>

namespace walle {
namespace wlevel {

Options::Options()
    : comparator(BytewiseComparator()),
      create_if_missing(false),
      error_if_exists(false),
      paranoid_checks(false),
      env(Env::Default()),
      info_log(NULL),
      write_buffer_size(4<<20),
      max_open_files(1000),
      block_cache(NULL),
      block_size(4096),
      block_restart_interval(16),
      compression(kSnappyCompression),
      filter_policy(NULL) {
}

}
}  
