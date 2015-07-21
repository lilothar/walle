#ifndef WALLE_LOGLEVEL_H_
#define WALLE_LOGLEVEL_H_

#include <string>
using std::string;

namespace walle {
namespace log {

class LogLevel{
	public:
		LogLevel();
		~LogLevel();
		bool setup(const string &ling);
		string toString();
	public:
		 int    _level;
		 string _upcaseStr;
		 string _lowcaseStr;
		
};

}
}
#endif