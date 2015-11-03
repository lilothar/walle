#ifndef WALLE_SYS_MD5_H_
#define WALLE_SYS_MD5_H_
#include <string>
#include <stdint.h>
namespace walle {

namespace util{
/* Type define */
typedef unsigned char byte;

using std::string;
using std::ifstream;

/* MD5 declaration. */
class MD5 {
public:
	MD5();
	MD5(const void *input, size_t length);
	MD5(const string &str);
	void update(const void *input, size_t length);
	void update(const string &str);
	const byte* digest();
	string toString();
	void reset();
private:
	void update(const byte *input, size_t length);
	void final();
	void transform(const byte block[64]);
	void encode(const uint32_t *input, byte *output, size_t length);
	void decode(const byte *input, uint32_t *output, size_t length);
	string bytesToHexString(const byte *input, size_t length);

	/* class uncopyable */
	MD5(const MD5&);
	MD5& operator=(const MD5&);
private:
	uint32_t _state[4];	/* state (ABCD) */
	uint32_t _count[2];	/* number of bits, modulo 2^64 (low-order word first) */
	byte     _buffer[64];	/* input buffer */
	byte     _digest[16];	/* message digest */
	bool     _finished;		/* calculate finished ? */

	static const byte PADDING[64];	/* padding for calculate */
	static const char HEX[16];
	static const size_t BUFFER_SIZE = 1024;
};
          
}
}
#endif
