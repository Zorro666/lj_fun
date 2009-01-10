#include "LJ_mem.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: Provides a suite of replacement functions for stdlib memory functions and adds some other useful ones.
//
/////////////////////////////////////////////////////////////////////////////////////////////////

// Not optimal - to be better should do bytes, then shorts, then words, then longs, then quadwords as atomic memory copies
void LJ_memCopy( void* const to, const void* const from, const LJ_uint numBytes )
{
	LJ_uint i;
	LJ_uint8* const toPtr = to;
	const LJ_uint8* const fromPtr = from;
	for ( i = 0; i < numBytes; i++ )
	{
		toPtr[i] = fromPtr[i];
	}
}

void LJ_memSet( void* const to, const LJ_uint8 value, const LJ_uint numBytes )
{
	LJ_uint i;
	LJ_uint8* const charPtr = to;
	for ( i = 0; i < numBytes; i++ )
	{
		charPtr[i] = value;
	}
}

void LJ_memZero( void* const to, const LJ_uint numBytes )
{
	LJ_memSet( to, 0, numBytes );
}

