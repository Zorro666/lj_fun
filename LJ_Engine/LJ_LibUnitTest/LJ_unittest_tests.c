#include "LJ.h"
#include "LJ_unittest.h"
#include <math.h>

// Put all the engine unit test register macros in here
#if LJ_USE_UNITTEST

void LJ_unittestRegisterExternalTests( void )
{
	// Do these first because all other tests rely on them
	// LJ_unittest.h API tests
	LJ_UNITTEST_REGISTER( unittest, macros );

	// Do the mem tests before str tests because the str tests rely on the LJ_mem functions
	// LJ_mem.h API tests
	LJ_UNITTEST_REGISTER( mem, LJ_memSet );
	LJ_UNITTEST_REGISTER( mem, LJ_memZero );
	LJ_UNITTEST_REGISTER( mem, LJ_memCopy );

	// LJ_str.h API tests
	LJ_UNITTEST_REGISTER( str, fundamental );
	LJ_UNITTEST_REGISTER( str, compare );
	LJ_UNITTEST_REGISTER( str, find );
	LJ_UNITTEST_REGISTER( str, convert );
	LJ_UNITTEST_REGISTER( str, utf8 )
	LJ_UNITTEST_REGISTER( str, misc )
	LJ_UNITTEST_REGISTER( str, character )
	LJ_UNITTEST_REGISTER( str, slash )
}

// Unittest unittests (uses itself to test things)
LJ_UNITTEST_FUNCTION_START( unittest, macros )
{
	LJ_UNITTEST_FLOAT_EQUALS( 1.0f, 1.0001f, 1.0e-3f );
}
LJ_UNITTEST_FUNCTION_END( unittest, macros )


#endif // #if LJ_USE_UNITTEST

