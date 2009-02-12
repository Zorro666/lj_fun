#ifndef LJ_MEMDEBUG_H
#define LJ_MEMDEBUG_H

#include "LJ.h"

#if LJ_USE_MEM_TRACKING

#include "LJ_LibCommon/LJ_Base/LJ_types.h"
#include "LJ_LibCommon/LJ_Mem/LJ_mem.h"

void LJ_memDebugInit( void );
void LJ_memDebugReset( void );
void LJ_memDebugShutdown( void );

// Add an allocation into the debug list
LJ_bool LJ_memDebugAddAllocation( const void* const address, const LJ_memHeapHandle heapHandle, 
		 						  const LJ_uint elementSize, const LJ_uint numElements, const LJ_uint size, 
			   					  const LJ_char* const file, const LJ_uint line, const LJ_char* const func, const LJ_char* const type );

LJ_bool LJ_memDebugRemoveAllocation( const void* const address );

// Find the named memory allocation (by ptr)
struct LJ_memDebugInfo* LJ_memDebugFindAllocation( const void* const address );

// Free the specified memory allocation
void LJ_memDebugResetAllocation( struct LJ_memDebugInfo* const memDebugInfo );

#endif // #if LJ_USE_MEM_TRACKING

#endif // #ifndef LJ_MEMDEBUG_H


