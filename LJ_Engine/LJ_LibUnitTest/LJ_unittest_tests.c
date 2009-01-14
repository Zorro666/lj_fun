#include "LJ.h"
#include "LJ_unittest.h"
#include <math.h>
#include <stdlib.h>

// Put all the engine unit test register macros in here
#if LJ_USE_UNITTEST

void LJ_unittestRegisterExternalTests( void )
{
	// Do these first because all other tests rely on them
	// LJ_unittest.h API tests
	LJ_UNITTEST_REGISTER( unittest, macros );
	LJ_UNITTEST_REGISTER( unittest, order1 );
	LJ_UNITTEST_REGISTER( unittest, order2 );
	LJ_UNITTEST_REGISTER( unittest, order3 );

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

static LJ_int s_unittestOrder = -1;

// Unittest unittests (uses itself to test things)
LJ_UNITTEST_FUNCTION_START( unittest, macros )
{
	#define LJ_UNITTEST_MAX_NUM_TESTS 1024
	#define LJ_UNITTEST_MAX_VALUE 65536
	#define LJ_UNITTEST_FLOAT_TOLERANCE 1.0e-3f
	LJ_int i;
	LJ_UNITTEST_TRUE( LJ_TRUE );
	for ( i = 0; i < LJ_UNITTEST_MAX_NUM_TESTS; i++ )
	{
		LJ_uint lhsUint;
		LJ_uint rhsUint;
		LJ_int lhsInt;
		LJ_int rhsInt;
		LJ_float lhsFloat;
		LJ_float rhsFloat;
		LJ_uint rnd;

		rnd = rand();
		lhsUint = rnd % LJ_UNITTEST_MAX_VALUE;
		rhsUint = lhsUint;
		LJ_UNITTEST_EQUALS( lhsUint, rhsUint );

		rnd = rand();
		lhsInt = rnd % LJ_UNITTEST_MAX_VALUE;
		rhsInt = lhsInt;
		LJ_UNITTEST_EQUALS( lhsInt, rhsInt );

		rnd = rand();
		lhsFloat = (LJ_float)( rnd % LJ_UNITTEST_MAX_VALUE ) / 123.0f;
		rhsFloat = lhsFloat + ( LJ_UNITTEST_FLOAT_TOLERANCE * 0.5f );
		LJ_UNITTEST_FLOAT_EQUALS( lhsFloat, rhsFloat, LJ_UNITTEST_FLOAT_TOLERANCE );
	}
	s_unittestOrder = 0;
}
LJ_UNITTEST_FUNCTION_END( unittest, macros )

LJ_UNITTEST_FUNCTION_START( unittest, order1 )
{
	LJ_UNITTEST_EQUALS( s_unittestOrder, 0 );
	s_unittestOrder = 1;
}
LJ_UNITTEST_FUNCTION_END( unittest, order1 )

LJ_UNITTEST_FUNCTION_START( unittest, order2 )
{
	LJ_UNITTEST_EQUALS( s_unittestOrder, 1 );
	s_unittestOrder = 2;
}
LJ_UNITTEST_FUNCTION_END( unittest, order2 )

LJ_UNITTEST_FUNCTION_START( unittest, order3 )
{
	LJ_UNITTEST_EQUALS( s_unittestOrder, 2 );
	s_unittestOrder = 3;
}
LJ_UNITTEST_FUNCTION_END( unittest, order3 )

#endif // #if LJ_USE_UNITTEST

