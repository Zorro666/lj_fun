#include "LJ_types.h"

#include <stdio.h>

void LJ_typesInit( void )
{
	printf( "sizeof(LJ_int) = %d\n", sizeof( LJ_int ) );
	printf( "sizeof(LJ_uint) = %d\n", sizeof( LJ_uint ) );
	printf( "sizeof(LJ_long) = %d\n", sizeof( LJ_long ) );
	printf( "sizeof(LJ_ulong) = %d\n", sizeof( LJ_ulong ) );
	printf( "sizeof(LJ_float) = %d\n", sizeof( LJ_float ) );
	printf( "sizeof(LJ_double) = %d\n", sizeof( LJ_double ) );
}

