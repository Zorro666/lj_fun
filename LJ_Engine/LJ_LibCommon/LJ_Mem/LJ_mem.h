#ifndef LJ_MEM_H
#define LJ_MEM_H

// internal dependencies
#include "LJ_LibCommon/LJ_Base/LJ_types.h"

void LJ_memInit( void );
void LJ_memReset( void );
void LJ_memShutdown( void );

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: Provides a suite of replacement functions for stdlib memory functions and adds some other useful ones.
//
/////////////////////////////////////////////////////////////////////////////////////////////////

void LJ_memCopy( void* const to, const void* const from, const LJ_uint numBytes );

void LJ_memSet( void* const to, const LJ_uint8 value, const LJ_uint numBytes );
void LJ_memZero( void* const to, const LJ_uint numBytes );

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// For allocation/deallocation wrapping
//
/////////////////////////////////////////////////////////////////////////////////////////////////

typedef LJ_uint LJ_memHeapHandle;
#define LJ_MEM_HEAP_INVALID		(-1U)

#ifndef LJ_MEM_ALLOC_MAX_NUM_ACTIVE_POINTERS
#define LJ_MEM_ALLOC_MAX_NUM_ACTIVE_POINTERS (20*1024)
#endif // #ifndef LJ_MEM_ALLOC_MAX_NUM_ACTIVE_POINTERS

LJ_EXTERN_INLINE LJ_memHeapHandle LJ_memGetSystemAllocHandle( void );

LJ_EXTERN_INLINE LJ_memHeapHandle LJ_memGetSystemAllocHandle( void )
{
    extern LJ_memHeapHandle g_memSystemAllocHandle;
    return g_memSystemAllocHandle;
}

#endif // #ifndef LJ_MEM_H

