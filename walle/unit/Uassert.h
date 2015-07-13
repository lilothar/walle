#ifndef U_ASSERT_H_
#define U_ASSERT_H_
#include <sstream>

#define TEST_FAIL(msg) \
	{																\
		assertment(::unit::Source(__FILE__, __LINE__, (msg) != 0 ? #msg : "")); \
		if (!continue_after_failure()) return;						\
	}


#define TEST_ASSERT(expr)  											\
	{  																\
		if (!(expr))  												\
		{ 															\
			assertment(::unit::Source(__FILE__, __LINE__, #expr));	\
			if (!continue_after_failure()) return;					\
		} 															\
	}

#define TEST_ASSERT_MSG(expr, msg)									\
	{  																\
		if (!(expr))  												\
		{ 															\
			assertment(::unit::Source(__FILE__, __LINE__, msg));	\
			if (!continue_after_failure()) return;					\
		} 															\
	}


#define TEST_ASSERT_EQUALS(expected, got)								\
	{																	\
		if (!((got) == (expected)))										\
		{																\
			std::stringstream tmpstream;								\
			tmpstream << "Got " << (got) << ", expected " << (expected);\
			assertment(::unit::Source(__FILE__, __LINE__,				\
						tmpstream.str().c_str()));						\
			if (!continue_after_failure()) return;						\
		}																\
	}


#define TEST_ASSERT_EQUALS_OBJ(expected, got)						\
	{																\
		if (!((got) == (expected)))									\
		{															\
			std::stringstream tmpstream;							\
			tmpstream << #expected << " object not equal to ";		\
			tmpstream << #got << " object.";						\
			assertment(::unit::Source(__FILE__, __LINE__, 			\
						tmpstream.str().c_str()));					\
			if (!continue_after_failure()) return;					\
		}															\
	}


#define TEST_ASSERT_EQUALS_MSG(expected, got, msg)						\
	{																	\
		if (!((got) == (expected)))										\
		{																\
			std::stringstream tmpstream;								\
			tmpstream << (msg) << ": ";									\
			tmpstream << "Got " << (got) << ", expected " << (expected);\
			assertment(::unit::Source(__FILE__, __LINE__,				\
						tmpstream.str().c_str()));						\
			if (!continue_after_failure()) return;						\
		}																\
	}


#define TEST_ASSERT_DELTA(a, b, delta)								\
	{																\
		if (((b) < (a) - (delta)) || ((b) > (a) + (delta)))			\
		{															\
			assertment(::unit::Source(__FILE__, __LINE__, 			\
					   "delta(" #a ", " #b ", " #delta ")" ));		\
			if (!continue_after_failure()) return;					\
		}															\
	}

#define TEST_ASSERT_DELTA_MSG(a, b, delta, msg)						\
	{																\
		if (((b) < (a) - (delta)) || ((b) > (a) + (delta)))			\
		{															\
			assertment(::unit::Source(__FILE__, __LINE__, msg));	\
			if (!continue_after_failure()) return;					\
		}															\
	}
	

#define TEST_THROWS(expr, x)										\
	{																\
		bool __expected = false;									\
		try { expr; } 												\
		catch (x)			{ __expected = true; }					\
		catch (...)			{}										\
		if (!__expected)											\
		{															\
			assertment(::unit::Source(__FILE__, __LINE__, #expr));	\
			if (!continue_after_failure()) return;					\
		}															\
	}


#define TEST_THROWS_MSG(expr, x, msg)								\
	{																\
		bool __expected = false;									\
		try { expr; } 												\
		catch (x)			{ __expected = true; }					\
		catch (...)			{}										\
		if (!__expected)											\
		{															\
			assertment(::unit::Source(__FILE__, __LINE__, msg));	\
			if (!continue_after_failure()) return;					\
		}															\
	}


#define TEST_THROWS_ANYTHING(expr)									\
	{																\
		bool __expected = false;									\
		try { expr; } 												\
		catch (...) { __expected = true; }							\
		if (!__expected)											\
		{															\
			assertment(::unit::Source(__FILE__, __LINE__, #expr));	\
			if (!continue_after_failure()) return;					\
		}															\
	}


#define TEST_THROWS_ANYTHING_MSG(expr, msg)							\
	{																\
		bool __expected = false;									\
		try { expr; }		 										\
		catch (...) { __expected = true; }							\
		if (!__expected)											\
		{															\
			assertment(::unit::Source(__FILE__, __LINE__, msg));	\
			if (!continue_after_failure()) return;					\
		}															\
	}
	

#define TEST_THROWS_NOTHING(expr)									\
	{																\
		bool __expected = true;										\
		try { expr; } 												\
		catch (...) { __expected = false; }							\
		if (!__expected)											\
		{															\
			assertment(::unit::Source(__FILE__, __LINE__, #expr));	\
			if (!continue_after_failure()) return;					\
		}															\
	}


#define TEST_THROWS_NOTHING_MSG(expr, msg)							\
	{																\
		bool __expected = true;										\
		try { expr; } 												\
		catch (...) { __expected = false; }							\
		if (!__expected)											\
		{															\
			assertment(::unit::Source(__FILE__, __LINE__, msg));	\
			if (!continue_after_failure()) return;					\
		}															\
	}


#endif
