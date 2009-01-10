#ifndef LJ_MEM_H
#define LJ_MEM_H

// internal dependencies
#include "LJ_types.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: Provides a suite of replacement functions for stdlib memory functions and adds some other useful ones.
//
/////////////////////////////////////////////////////////////////////////////////////////////////

void LJ_memCopy( void* const to, const void* const from, const LJ_uint numBytes );

void LJ_memSet( void* const to, const LJ_uint8 value, const LJ_uint numBytes );
void LJ_memZero( void* const to, const LJ_uint numBytes );

#endif // #ifndef LJ_MEM_H
