#include "LJ.h"
#include "LJ_unittest.h"
#include <math.h>

// Put all the engine unit test register macros in here
#if LJ_USE_UNITTEST

void LJ_unittestRegisterExternalTests( void )
{
	LJ_UNITTEST_REGISTER( unittest, macros );
	LJ_UNITTEST_REGISTER( str, basic );
	LJ_UNITTEST_REGISTER( mem, LJ_memSet );
	LJ_UNITTEST_REGISTER( mem, LJ_memZero );
	LJ_UNITTEST_REGISTER( mem, LJ_memCopy );
}

// Unittest unittests (uses itself to test things)
LJ_UNITTEST_FUNCTION_START( unittest, macros )
{
	LJ_UNITTEST_FLOAT_EQUALS( 1.0f, 1.0001f, 1.0e-3f );
}
LJ_UNITTEST_FUNCTION_END( unittest, macros )


#endif // #if LJ_USE_UNITTEST

