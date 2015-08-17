#ifndef WALLE_STORE_MYCONF_H  
#define WALLE_STORE_MYCONF_H

#define MYPATHCHR      '/'
#define MYPATHSTR      "/"
#define MYEXTCHR       '.'
#define MYEXTSTR       "."
#define MYCDIRSTR      "."
#define MYPDIRSTR      ".."


extern "C" {
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
}

extern "C" {
#include <stdint.h>
}


extern "C" {
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <dirent.h>
}

extern "C" {
#include <pthread.h>
#include <sched.h>
}

#endif         

