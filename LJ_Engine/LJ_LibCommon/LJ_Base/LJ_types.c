#include "LJ.h"

#include "LJ_types.h"
#include "LJ_assert.h"

void LJ_typesInit( void )
{
	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_char) >= 1 );
	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_uchar) >= 1 );

	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_short) >= 2 );
	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_ushort) >= 2 );

	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_int) >= 4 );
	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_uint) >= 4 );

	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_long) >= 8 );
	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_ulong) >= 8 );

	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_int8) == 1 );
	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_uint8) == 1 );

	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_int16) == 2 );
	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_uint16) == 2 );

	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_int32) == 4 );
	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_uint32) == 4 );

	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_int64) == 8 );
	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_uint64) == 8 );

	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_float) >= 4 );
	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_double) >= 8 );

	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_float32) == 4 );
	LJ_COMPILE_TIME_ASSERT( sizeof(LJ_float64) == 8 );
}

