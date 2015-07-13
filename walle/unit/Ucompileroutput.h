#ifndef U_COMPILEROUTPUT_H_
#define U_COMPILEROUTPUT_H_
#include <iostream>
#include <stdexcept>

#include "Uoutput.h"

namespace unit
{

	class CompilerOutput : public Output
	{
	public:

		class InvalidFormat : public std::logic_error
		{
			public:
				InvalidFormat(const std::string& spwhat)
					: std::logic_error(spwhat) {}
		};
		

		enum Format
		{

			Generic,
			GCC,

		};
		
		explicit CompilerOutput(Format 				format = Generic,
								std::ostream& 		stream = std::cout);
		
		explicit CompilerOutput(const std::string&	format,
								std::ostream& 		stream = std::cout);
		
		virtual void assertment(const Source& s);
		
	private:
		std::string		_format;
		std::ostream&	_stream;
	};

} 

#endif
