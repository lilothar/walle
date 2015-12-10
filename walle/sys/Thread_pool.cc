#include <walle/sys/Thread_pool.h>
#include <assert.h>
#include <stdio.h>
#include <walle/thread/ScopeLock.h>
#include <algorithm>

namespace walle{
namespace sys {

ThreadPool::ThreadPool(const string& nameArg)
  : _mutex(),
    _notEmpty(),
    _notFull(),
    _name(nameArg),
    _maxQueueSize(0),
    _running(false)
{
}

ThreadPool::~ThreadPool()
{
  if (_running)
  {
    stop();
	for(size_t i = 0; i <_threads.size(); i++ ) {
		delete _threads[i];
		_threads[i] = NULL;
	}
	_threads.clear();
  }
}

void ThreadPool::start(int numThreads)
{
  assert(_threads.empty());
  _running = true;
  _threads.reserve(numThreads);
  for (int i = 0; i < numThreads; ++i)
  {
    char id[32];
    snprintf(id, sizeof id, "%d", i+1);
    _threads.push_back(new walle::thread::Thread(
          std::bind(&ThreadPool::runInThread, this), _name+id));
    _threads[i]->start();
  }
  if (numThreads == 0 && _threadInitCallback)
  {
    _threadInitCallback();
  }
}

void ThreadPool::stop()
{
  {
  ScopeMutex lock(&_mutex);
  _running = false;
  _notEmpty.broadcast();
  }
  std::for_each(_threads.begin(),
           _threads.end(),
           std::bind(&walle::thread::Thread::join, std::_1));
}

size_t ThreadPool::queueSize() const
{
  ScopeMutex lock(&_mutex);
  return _queue.size();
}

void ThreadPool::run(const Task& task)
{
  if (_threads.empty())
  {
    task();
  }
  else
  {
    ScopeMutex lock(&_mutex);
    while (isFull())
    {
      _notFull.wait(_mutex);
    }
    assert(!isFull());

    _queue.push_back(task);
    _notEmpty.broadcast();
  }
}

ThreadPool::Task ThreadPool::take()
{
  ScopeMutex lock(&_mutex);
  // always use a while-loop, due to spurious wakeup
  while (_queue.empty() && _running)
  {
    _notEmpty.wait(_mutex);
  }
  Task task;
  if (!_queue.empty())
  {
    task = _queue.front();
    _queue.pop_front();
    if (_maxQueueSize > 0)
    {
      _notFull.broadcast();
    }
  }
  return task;
}

bool ThreadPool::isFull() const
{
  return _maxQueueSize > 0 && _queue.size() >= _maxQueueSize;
}

void ThreadPool::runInThread()
{

    if (_threadInitCallback)
    {
      _threadInitCallback();
    }
    while (_running)
    {
      Task task(take());
      if (task)
      {
        task();
      }
    }
}
}
}
