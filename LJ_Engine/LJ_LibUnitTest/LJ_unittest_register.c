#include "LJ.h"
#include "LJ_unittest.h"

// Put all the engine unit test register macros in here
#if LJ_USE_UNITTEST

void LJ_unittestRegisterExternalTests( void )
{
	// Do these first because all other tests rely on them
	// LJ_LibUnittest/LJ_unittest.h API tests
	LJ_UNITTEST_REGISTER( unittest, macros );
	LJ_UNITTEST_REGISTER( unittest, order1 );
	LJ_UNITTEST_REGISTER( unittest, order2 );
	LJ_UNITTEST_REGISTER( unittest, order3 );

	// Do the mem tests before str tests because the str tests rely on the LJ_mem functions
	// LJ_LibCommon/LJ_Base/LJ_mem.h API tests
	LJ_UNITTEST_REGISTER( mem, LJ_memSet );
	LJ_UNITTEST_REGISTER( mem, LJ_memZero );
	LJ_UNITTEST_REGISTER( mem, LJ_memCopy );
	LJ_UNITTEST_REGISTER( mem, alloc_free );

	// LJ_LibCommon/LJ_Base/LJ_str.h API tests
	LJ_UNITTEST_REGISTER( str, fundamental );
	LJ_UNITTEST_REGISTER( str, compare );
	LJ_UNITTEST_REGISTER( str, find );
	LJ_UNITTEST_REGISTER( str, convert );
	LJ_UNITTEST_REGISTER( str, utf8 )
	LJ_UNITTEST_REGISTER( str, misc )
	LJ_UNITTEST_REGISTER( str, character )
	LJ_UNITTEST_REGISTER( str, slash )
	
	// LJ_LibCommon/LJ_Math/LJ_math.h API tests
	LJ_UNITTEST_REGISTER( math, basic )
	LJ_UNITTEST_REGISTER( math, random )
}

#endif // #if LJ_USE_UNITTEST

