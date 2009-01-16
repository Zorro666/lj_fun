#include "LJ.h"
#include "LJ_unittest.h"
#include "LJ_LibCommon/LJ_LibCommon.h"

#if LJ_USE_UNITTEST

#define LJ_MEM_TEST_SIZE 128

LJ_UNITTEST_FUNCTION_START( mem, LJ_memSet )
{
	LJ_char buffer1[LJ_MEM_TEST_SIZE];
	LJ_char buffer2[LJ_MEM_TEST_SIZE];
	LJ_int i;
	LJ_int badIndex = -1;
	for ( i = 0; i < LJ_MEM_TEST_SIZE; i++ )
	{
		buffer1[i] = 0xCD;
	}
	LJ_memSet( buffer2, 0xCD, LJ_MEM_TEST_SIZE );
	for ( i = 0; i < LJ_MEM_TEST_SIZE; i++ )
	{
		if ( buffer1[i] != buffer2[i] )
		{
			badIndex = i;
			break;
		}
	}
	LJ_UNITTEST_TRUE( badIndex == -1 );
}
LJ_UNITTEST_FUNCTION_END( mem, LJ_memSet )

LJ_UNITTEST_FUNCTION_START( mem, LJ_memZero )
{
	LJ_char buffer1[LJ_MEM_TEST_SIZE];
	LJ_int i;
	LJ_int badIndex = -1;
	for ( i = 0; i < LJ_MEM_TEST_SIZE; i++ )
	{
		buffer1[i] = 0xCD;
	}
	LJ_memZero( buffer1, LJ_MEM_TEST_SIZE );
	for ( i = 0; i < LJ_MEM_TEST_SIZE; i++ )
	{
		if ( buffer1[i] != 0x0 )
		{
			badIndex = i;
			break;
		}
	}
	LJ_UNITTEST_TRUE( badIndex == -1 );
}
LJ_UNITTEST_FUNCTION_END( mem, LJ_memZero )

LJ_UNITTEST_FUNCTION_START( mem, LJ_memCopy )
{
	LJ_char buffer1[LJ_MEM_TEST_SIZE];
	LJ_char buffer2[LJ_MEM_TEST_SIZE];
	LJ_int i;
	LJ_int badIndex = -1;
	for ( i = 0; i < LJ_MEM_TEST_SIZE; i++ )
	{
		buffer1[i] = ( i & 0xFF );
	}
	LJ_memCopy( buffer2, buffer1, LJ_MEM_TEST_SIZE );
	for ( i = 0; i < LJ_MEM_TEST_SIZE; i++ )
	{
		if ( buffer1[i] != buffer2[i] )
		{
			badIndex = i;
			break;
		}
	}
	LJ_UNITTEST_TRUE( badIndex == -1 );
}
LJ_UNITTEST_FUNCTION_END( mem, LJ_memCopy )

#endif // #if LJ_USE_UNITTEST

