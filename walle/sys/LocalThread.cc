#include <walle/sys/LocalThread.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

namespace walle {
namespace LocalThread{
	  __thread pthread_t t_tid = 0;
	  __thread char t_tidString[32];
	  __thread size_t t_tidStringLength= 0;
      __thread char t_threadName[32]="";
	
    void setThreadName(const char* str)
    {
        if(!str) {
          snprintf(t_threadName,15, "%s", "unknown");  
        }
             
        snprintf(t_threadName,15, "%s", str);
    }
	void cacheTid()
	{
	  if (t_tid == 0) {
	    	t_tid = ::pthread_self();
	   		 t_tidStringLength = snprintf(t_tidString, sizeof(t_tidString), "%lu", 
			t_tid);
            t_tidStringLength = strlen(t_tidString); 
            setThreadName("unknown");
	  }
	}
	bool isMainThread()
	{
 		  return tid() == (pthread_t)::getpid();
	}


}


}
