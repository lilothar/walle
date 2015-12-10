#include <walle/sys/Asynclog.h>
#include <walle/sys/LogFile.h>
#include <walle/sys/Time.h>
#include <walle/sys/Types.h>
#include <stdio.h>

using namespace walle::sys;

AsyncLogging::AsyncLogging(const string& basename,
                           size_t rollSize,
                           int flushInterval)
  : _flushInterval(flushInterval),
  	_isrunning(false),
    _basename(basename),
    _rollSize(rollSize),
    _latch(1),
    _mutex(),
    _cond(),
    _currentBuffer(new Buffer),
    _nextBuffer(new Buffer),
    _buffers(),
    _th(std::bind(&AsyncLogging::run,this),"async-log")
{
  
  _currentBuffer->bzero();
  _nextBuffer->bzero();
  _buffers.reserve(16);
}

void AsyncLogging::append(const char* logline, size_t len)
{
	ScopeMutex lock(&_mutex);
  	if (implicit_cast<size_t>(_currentBuffer->avail()) > len) {
		_currentBuffer->append(logline, len);
  	} else {
    	_buffers.push_back(_currentBuffer);
		_currentBuffer = NULL;

    	if (_nextBuffer)  {
     		_currentBuffer = _nextBuffer;
	  		_nextBuffer = NULL;
		} else {
      		_currentBuffer = new Buffer; // Rarely happens
		}	
    	_currentBuffer->append(logline, len);
   	 	_cond.signal();
  	}
}

void AsyncLogging::run()
{
  _latch.down();
  LogFile output(_basename, _rollSize, false);
  Buffer *newBuffer1 = new Buffer;
  Buffer *newBuffer2 = new Buffer;
  newBuffer1->bzero();
  newBuffer2->bzero();
  BufferVector buffersToWrite;
  buffersToWrite.reserve(16);
  while (_isrunning)
  {
    assert(newBuffer1 && newBuffer1->size() == 0);
    assert(newBuffer2 && newBuffer2->size() == 0);
    assert(buffersToWrite.empty());

    {
      ScopeMutex lock(&_mutex);
      if (_buffers.empty())  // unusual usage!
      {
        _cond.wait(&_mutex, _flushInterval*1000000);
      }
      _buffers.push_back(_currentBuffer);
      _currentBuffer = newBuffer1;
	  newBuffer1 = NULL;
      buffersToWrite.swap(_buffers);
      if (!_nextBuffer)
      {
        _nextBuffer = newBuffer2;
		newBuffer2 = NULL;
      }
    }

    if (buffersToWrite.size() > 25)
    {
      char buf[256];
      snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
               Time::now().toDateTime().c_str(),
               buffersToWrite.size()-2);
      fputs(buf, stderr);
      output.append(buf, static_cast<int>(strlen(buf)));
	  for(size_t i = 2 ; i < buffersToWrite.size(); i++ ) {
			delete 	buffersToWrite[i];
	  }
      buffersToWrite.resize(2);
    }
    size_t s = buffersToWrite.size();
    for (size_t i = 0; i < s; ++i)
    {
      // FIXME: use unbuffered stdio FILE ? or use ::writev ?
      output.append(buffersToWrite[i]->data(), buffersToWrite[i]->size());
    }

    if (buffersToWrite.size() > 2)
    {
      for(size_t i = 2 ; i < buffersToWrite.size(); i++ ) {
			delete 	buffersToWrite[i];
	  }
      buffersToWrite.resize(2);
    }

    if (!newBuffer1)
    {
      assert(!buffersToWrite.empty());
      newBuffer1 = buffersToWrite[0];
      newBuffer1->reset();
    }

    if (!newBuffer2)
    {
      assert(buffersToWrite.size() == 2);
      newBuffer2 = buffersToWrite[1];
      newBuffer2->reset();
    }

    buffersToWrite.clear();
    output.flush();
  }
  if(newBuffer1) {
	delete newBuffer1;
	newBuffer1 = NULL;
  }

  if(newBuffer2) {
	delete newBuffer2;
	newBuffer2 = NULL;
  }
  flushall(&output);
}
void AsyncLogging::flushall(LogFile *output)
{

		 ScopeMutex lock(&_mutex);
		 _buffers.push_back(_currentBuffer);
		 _currentBuffer = NULL;
		 if (!_buffers.empty()) {
		 	for (size_t i = 0; i <_buffers.size(); i++) {
				output->append(_buffers[i]->data(), _buffers[i]->size());
				delete _buffers[i];
		 	}
		 }
	
	   output->flush();

}

