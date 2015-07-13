#include "Uutils.h"

namespace unit{
int
correct(int tests, int errors)
{
	if ((errors == 0) || (tests == 0))
		return 100;

	return (tests - errors) * 100 / tests;
}
}
