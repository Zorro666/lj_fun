#include "LJ.h"
#include "LJ_unittest.h"

// Put the unit test tests in here
#if LJ_USE_UNITTEST

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

		rnd = LJ_mathGetRand32();
		lhsUint = rnd % LJ_UNITTEST_MAX_VALUE;
		rhsUint = lhsUint;
		LJ_UNITTEST_EQUALS( lhsUint, rhsUint );

		rnd = LJ_mathGetRand32();
		lhsInt = rnd % LJ_UNITTEST_MAX_VALUE;
		rhsInt = lhsInt;
		LJ_UNITTEST_EQUALS( lhsInt, rhsInt );

		rnd = LJ_mathGetRand32();
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

