#include <walle/sys/wallesys.h>
#include <iostream>
#include <stdio.h>
#include <sys/resource.h>
using namespace std;
int kRollSize = 1000*1000*100;
using namespace walle::sys;
AsyncLogging* g_asyncLog = NULL;

void asyncOutput(const char* msg, size_t len)
{
  g_asyncLog->append(msg, len);
}

void bench(bool longLog)
{
  Logger::setOutput(asyncOutput);

  int cnt = 0;
  const int kBatch = 1000;
  string empty = " ";
  string longStr(3000, 'X');
  longStr += " ";

  for (int t = 0; t < 3000; ++t) {
    Time start = Time::now();
    for (int i = 0; i < kBatch; ++i) {
      LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz "
               << (longLog ? longStr : empty)
               << cnt;
      ++cnt;
    }
    Time end = Time::now();
    //cout<<start.toMicroSeconds()<<" "<<end.toMicroSeconds()<<endl;
    Time diff = end - start;
    cout<<diff.toMicroSecondsDouble()/kBatch<<endl;
    //struct timespec ts = { 0, 500*1000*1000 };
    //nanosleep(&ts, NULL);
  }
}

int main(int argc, char* argv[])
{
  {
    // set max virtual memory to 2GB.
    size_t kOneGB = 1000*1024*1024;
    rlimit rl = { 2*kOneGB, 2*kOneGB };
    setrlimit(RLIMIT_AS, &rl);
  }

  printf("pid = %d\n", getpid());

  char name[256];
  strncpy(name, argv[0], 256);
  AsyncLogging log(::basename(name), kRollSize);
  log.waitStart();
  g_asyncLog = &log;

  bool longLog = argc > 1;
  bench(longLog);
  log.stop();
  return 0;
}

