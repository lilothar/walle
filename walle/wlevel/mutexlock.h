// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef WALLE_WLEVEL_MUTEXLOCK_H_
#define WALLE_WLEVEL_MUTEXLOCK_H_

#include <walle/wlevel/port.h>

namespace walle {
namespace wlevel {

class  MutexLock {
 public:
  explicit MutexLock(port::Mutex *mu)
      : mu_(mu)  {
    this->mu_->Lock();
  }
  ~MutexLock() { this->mu_->Unlock(); }

 private:
  port::Mutex *const mu_;
  // No copying allowed
  MutexLock(const MutexLock&);
  void operator=(const MutexLock&);
};

}  

}
#endif

