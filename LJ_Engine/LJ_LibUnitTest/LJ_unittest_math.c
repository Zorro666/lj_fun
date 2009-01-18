#include "LJ.h"
#include "LJ_unittest.h"
#include "LJ_LibCommon/LJ_LibCommon.h"

#if LJ_USE_UNITTEST

LJ_UNITTEST_FUNCTION_START( math, basic )
{
	LJ_UNITTEST_FLOAT_EQUALS( LJ_mathPowf( 10, 3 ), 1000.0f, 0.0f );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_mathPowf( 10, -3 ), 1.0e-3f, 1.0e-8f );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_mathPowf( 2, 3 ), 8.0f, 0.0f );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_mathPowf( 4, 3 ), 64.0f, 0.0f );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_mathPowf( 2, 16 ), 65536.0f, 0.0f );

	LJ_UNITTEST_FLOAT_EQUALS( LJ_mathFabsf( 1.0f ), 1.0f, 0.0f );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_mathFabsf( -1.0f ), 1.0f, 0.0f );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_mathFabsf( -1.5f ), 1.5f, 0.0f );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_mathFabsf( +2.5f ), 2.5f, 0.0f );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_mathFabsf( +0.0f ), 0.0f, 0.0f );
}
LJ_UNITTEST_FUNCTION_END( math, basic )

LJ_UNITTEST_FUNCTION_START( math, random )
{
	// So can't use the random function to test itself!
	// So run it lots of times and check it stays within bounds
#define LJ_UNITTEST_RANDOM_MAX_NUM 65536*16
#define LJ_UNITTEST_RANDOM_AVG_DELTA_TOLERANCE 2
	LJ_uint i;
	LJ_uint min = 0xFFFFFFFF;
	LJ_uint max = 0;
	LJ_int intMin;
	LJ_int intMax;
	LJ_float floatMin;
	LJ_float floatMax;

	LJ_mathSeedRand( 345 );

	for ( i = 0; i < LJ_UNITTEST_RANDOM_MAX_NUM; i++ )
	{
		const LJ_uint value = LJ_mathGetRand32();
		if ( value > max )
		{
			max = value;
		}
		if ( value < min )
		{
			min = value;
		}
	}

	LJ_UNITTEST_EQUALS( min, 0 );
	LJ_UNITTEST_EQUALS( max, 0xFFFFFFFF );
	
	intMin = 0x7FFFFFFF;
	intMax = 0;
#define LJ_UNITTEST_RANDOM_INT_MIN -100
#define LJ_UNITTEST_RANDOM_INT_MAX +843
	for ( i = 0; i < LJ_UNITTEST_RANDOM_MAX_NUM; i++ )
	{
		const LJ_int value = LJ_mathGetRandRange( LJ_UNITTEST_RANDOM_INT_MIN, LJ_UNITTEST_RANDOM_INT_MAX );
		if ( value > intMax )
		{
			intMax = value;
		}
		if ( value < intMin )
		{
			intMin = value;
		}
	}
	LJ_UNITTEST_EQUALS( intMin, LJ_UNITTEST_RANDOM_INT_MIN );
	LJ_UNITTEST_EQUALS( intMax, LJ_UNITTEST_RANDOM_INT_MAX );

#define LJ_UNITTEST_RANDOM_INT_MAX2 +1056
	intMin = 0x7FFFFFFF;
	intMax = 0;
	for ( i = 0; i < LJ_UNITTEST_RANDOM_MAX_NUM; i++ )
	{
		const LJ_int value = LJ_mathGetRandMax( LJ_UNITTEST_RANDOM_INT_MAX2 );
		if ( value > intMax )
		{
			intMax = value;
		}
		if ( value < intMin )
		{
			intMin = value;
		}
	}
	LJ_UNITTEST_EQUALS( intMin, 0 );
	LJ_UNITTEST_EQUALS( intMax, LJ_UNITTEST_RANDOM_INT_MAX2 );

	floatMin = 0xFFFFFFFF;
	floatMax = 0;
#define LJ_UNITTEST_RANDOM_FLOAT_MIN -100.01f
#define LJ_UNITTEST_RANDOM_FLOAT_MAX +843.56f
	for ( i = 0; i < LJ_UNITTEST_RANDOM_MAX_NUM; i++ )
	{
		const LJ_float value = LJ_mathGetRandRangeFloat( LJ_UNITTEST_RANDOM_FLOAT_MIN, LJ_UNITTEST_RANDOM_FLOAT_MAX );
		if ( value > floatMax )
		{
			floatMax = value;
		}
		if ( value < floatMin )
		{
			floatMin = value;
		}
	}
	LJ_UNITTEST_FLOAT_EQUALS( floatMin, LJ_UNITTEST_RANDOM_FLOAT_MIN, 1.0e-6f );
	LJ_UNITTEST_FLOAT_EQUALS( floatMax, LJ_UNITTEST_RANDOM_FLOAT_MAX, 1.0e-6f );

#define LJ_UNITTEST_RANDOM_FLOAT_MAX2 +1056.453f
	floatMin = 0xFFFFFFFF;
	floatMax = 0;
	for ( i = 0; i < LJ_UNITTEST_RANDOM_MAX_NUM; i++ )
	{
		const LJ_float value = LJ_mathGetRandMax( LJ_UNITTEST_RANDOM_FLOAT_MAX2 );
		if ( value > floatMax )
		{
			floatMax = value;
		}
		if ( value < floatMin )
		{
			floatMin = value;
		}
	}
	LJ_UNITTEST_FLOAT_EQUALS( floatMin, 0.0f, 1.0f );
	LJ_UNITTEST_FLOAT_EQUALS( floatMax, LJ_UNITTEST_RANDOM_FLOAT_MAX2, 1.0f );
// void LJ_mathSeedRand( const LJ_int seed );
}
LJ_UNITTEST_FUNCTION_END( math, random )

#endif // #if LJ_USE_UNITTEST

