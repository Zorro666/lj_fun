#include "LJ.h"

#include "LJ_Mem/LJ_mem.h"
#include "LJ_Mem/LJ_memAlloc.h"

void LJ_memInit( void )
{
	LJ_memAllocInit();
}

void LJ_memReset( void )
{
	LJ_memAllocReset();
}

void LJ_memShutdown( void )
{
	LJ_memAllocShutdown();
}

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

LJ_bool LJ_memIsSame( const void* const mem, const void* const compare, const LJ_uint numBytes )
{
	const LJ_char* readMemory = mem;
	const LJ_char* readCompare = compare;
	LJ_uint length = 0;

	if ( numBytes == 0 )
	{
		return LJ_TRUE;
	}

	while ( 1 )
	{
		if ( length == numBytes )
		{
			return LJ_TRUE;
		}
		else if ( *readMemory != *readCompare )
		{
			return LJ_FALSE;
		}
		readMemory++;
		readCompare++;
		length++;
	}
}

