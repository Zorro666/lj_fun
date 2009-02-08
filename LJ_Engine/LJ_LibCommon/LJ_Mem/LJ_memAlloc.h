#ifndef LJ_MEMALLOC_H
#define LJ_MEMALLOC_H

// internal dependencies
#include "LJ_LibCommon/LJ_Base/LJ_types.h"
#include "LJ_LibCommon/LJ_Mem/LJ_mem.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Allocation/Deallocation wrappers: 
//
// Use this to get memory debugging support e.g. leak detection on exit, error on double free of a ptr
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" 
{
#endif // #ifdef __cplusplus

void LJ_memAllocInit( void );
void LJ_memAllocReset( void );
void LJ_memAllocShutdown( void );

#define LJ_MEM_SYSTEM_ALLOC		(0xFFFFFFFEU)

void* LJ_memAllocInternal( LJ_memHeapHandle heapHandle, const LJ_uint elementSize, const LJ_uint numElements, LJ_uint* const allocSize );
LJ_bool LJ_memFreeInternal( void* const address );

#if LJ_USE_MEM_TRACKING

void* LJ_memAllocDebugInternal( LJ_memHeapHandle heapHandle, const LJ_uint elementSize, const LJ_uint numElements, 
			   					const LJ_char* const file, const LJ_uint line, 
								const LJ_char* const func, const LJ_char* const type );

LJ_bool LJ_memFreeDebugInternal( void* const address, 
			   				     const LJ_char* const file, const LJ_uint line, const LJ_char* const func );

#define LJ_MEM_ALLOC( heapHandle, type, numElements ) \
    (type*)LJ_memAllocDebugInternal( ( heapHandle ), sizeof( type ), numElements, LJ_SOURCE_FILE, LJ_SOURCE_LINE, LJ_SOURCE_FUNCTION, #type )

#define LJ_MEM_FREE( address ) \
    LJ_memFreeDebugInternal( ( address ), LJ_SOURCE_FILE, LJ_SOURCE_LINE, LJ_SOURCE_FUNCTION )

#else // #if LJ_USE_MEM_TRACKING

#define LJ_MEM_ALLOC( heapHandle, type, numElements ) \
    (type*)LJ_memAllocInternal( ( heapHandle ), sizeof( type ), numElements, LJ_NULL )

#define LJ_MEM_FREE( address ) \
    LJ_memFreeInternal( ( address ) )

#endif // #if LJ_USE_MEM_TRACKING

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif // #ifndef LJ_MEMALLOC_H

