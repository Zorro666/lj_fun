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
		buffer1[i] = (LJ_char)( i & 0xFF );
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

LJ_UNITTEST_FUNCTION_START( mem, alloc_free )
{
	// Not sure how to unittest the memory stuff - do some alloc/deallocs to cause a leak if this have gone wrong
	LJ_int* const memTest0 = LJ_MEM_ALLOC( LJ_memGetSystemAllocHandle(), LJ_int, 25 );
	LJ_float* const memTest1 = LJ_MEM_ALLOC( LJ_memGetSystemAllocHandle(), LJ_float, 10 );
	LJ_uint* const memTest2 = LJ_MEM_ALLOC( LJ_memGetSystemAllocHandle(), LJ_uint, 1 );
	LJ_floatIntUnion* memTest3 = LJ_NULL;

	LJ_UNITTEST_TRUE( LJ_MEM_FREE( memTest2 ) );
	LJ_UNITTEST_TRUE( LJ_MEM_FREE( memTest1 ) );

	memTest3 = LJ_MEM_ALLOC( LJ_memGetSystemAllocHandle(), LJ_floatIntUnion, 2 );

	LJ_UNITTEST_TRUE( LJ_MEM_FREE( memTest3 ) );
	LJ_UNITTEST_TRUE( LJ_MEM_FREE( memTest0 ) );
}
LJ_UNITTEST_FUNCTION_END( mem, alloc_free )

#if 0
LJ_UNITTEST_FUNCTION_START( mem, LJ_memIsSame )
{
	// LJ_bool LJ_memIsSame( const void* const mem, const void* const compare, const LJ_uint numBytes )
}
LJ_UNITTEST_FUNCTION_END( mem, LJ_memIsSame )
#endif // #if 0

#endif // #if LJ_USE_UNITTEST

