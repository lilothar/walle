#ifndef U_SOURCE_H_
#define U_SOURCE_H_
#include <string>

namespace unit{

	class Suite;
	
	class Source
	{
		friend class Suite;
		
	public:
		Source();
		Source(const char* spfile, unsigned int ipline, const char* spmsg);
		
		const std::string& file() const;
		unsigned int line() const;
		const std::string& message() const;
		const std::string& suite() const;
		const std::string& test() const;
		
	private:
		unsigned int	_line;
		std::string		_file;
		std::string		_msg;
		std::string		_suite;
		std::string		_test;
	};
	


}
#endif
