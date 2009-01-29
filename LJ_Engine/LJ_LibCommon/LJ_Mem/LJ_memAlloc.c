#include "LJ.h"

#include "LJ_Mem/LJ_memAlloc.h"
#include "LJ_Mem/LJ_memDebug.h"

#include "LJ_Base/LJ_output.h"

#include <malloc.h>

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Allocation/Deallocation wrappers: 
//
// Use this to get memory debugging support e.g. leak detection on exit, error on double free of a ptr
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef LJ_MEM_MAX_NUM_HEAPS
#define LJ_MEM_MAX_NUM_HEAPS	(32)
#endif // #ifndef LJ_MEM_MAX_NUM_HEAPS

typedef struct LJ_memAllocActivePointer
{
	const void* address;
	LJ_memHeapHandle heapHandle;
} LJ_memAllocActivePointer;

static LJ_memAllocActivePointer s_memAllocActivePointers[LJ_MEM_ALLOC_MAX_NUM_ACTIVE_POINTERS];
static LJ_uint s_memAllocNumActivePointers = 0;

typedef struct LJ_memHeap
{
    LJ_uint maxSize;
} LJ_memHeap;
//static LJ_memHeap s_memHeapArray[LJ_MEM_MAX_NUM_HEAPS];

LJ_memHeapHandle g_memSystemAllocHandle = LJ_MEM_SYSTEM_ALLOC;

void LJ_memAllocInit( void )
{
	LJ_uint i;
	for ( i = 0; i < LJ_MEM_ALLOC_MAX_NUM_ACTIVE_POINTERS; i++ )
	{
		s_memAllocActivePointers[i].address = LJ_NULL;
		s_memAllocActivePointers[i].heapHandle = LJ_MEM_HEAP_INVALID;
	}
	s_memAllocNumActivePointers = 0;

#if LJ_USE_MEM_TRACKING
	LJ_memDebugInit();
#endif // #if LJ_USE_MEM_TRACKING

}
void LJ_memAllocReset( void )
{
#if LJ_USE_MEM_TRACKING
	LJ_memDebugReset();
#endif // #if LJ_USE_MEM_TRACKING
}

void LJ_memAllocShutdown( void )
{
#if LJ_USE_MEM_TRACKING
	LJ_memDebugShutdown();
#endif // #if LJ_USE_MEM_TRACKING
}

// Add a pointer to the active pointer list
static LJ_bool  LJ_memAllocAddPointer( const void* address, const LJ_memHeapHandle heapHandle )
{
	LJ_uint i;
	for ( i = 0; i < LJ_MEM_ALLOC_MAX_NUM_ACTIVE_POINTERS; i++ )
	{
		LJ_memAllocActivePointer* const memAllocActivePointer = s_memAllocActivePointers + i;
		if ( memAllocActivePointer->address == LJ_NULL )
		{
		 	memAllocActivePointer->address = address;
		 	memAllocActivePointer->heapHandle = heapHandle;

			if ( i == s_memAllocNumActivePointers )
			{
				s_memAllocNumActivePointers++;
			}
			return LJ_TRUE;
		}
	}
	return LJ_FALSE;
}

// Returns the memory heap the pointer belongs to
static LJ_memHeapHandle LJ_memAllocFreePointer( const void* address )
{
	LJ_uint i;
	for ( i = 0; i < s_memAllocNumActivePointers; i++ )
	{
		LJ_memAllocActivePointer* const memAllocActivePointer = s_memAllocActivePointers + i;
		if ( memAllocActivePointer->address == address )
		{
			const LJ_memHeapHandle heapHandle = memAllocActivePointer->heapHandle;
			memAllocActivePointer->address = LJ_NULL;
			memAllocActivePointer->heapHandle = LJ_MEM_HEAP_INVALID;

			if ( i == ( s_memAllocNumActivePointers - 1 ) )
			{
				s_memAllocNumActivePointers = i;
			}

			return heapHandle;
		}
	}
	return LJ_MEM_HEAP_INVALID;
}

#if LJ_USE_MEM_TRACKING

void* LJ_memAllocDebugInternal( LJ_memHeapHandle heapHandle, const LJ_uint elementSize, const LJ_uint numElements, 
			   					const LJ_char* const file, const LJ_uint line, const LJ_char* const func, const LJ_char* const type )
{
	// Find a mem debug free slot
	LJ_uint size = 0;
	void* const address = LJ_memAllocInternal( heapHandle, elementSize, numElements, &size );

	LJ_memDebugAddAllocation( address, heapHandle, elementSize, numElements, size, file, line, func, type );

	return address;
}

LJ_bool LJ_memFreeDebugInternal( void* const address, 
			   				     const LJ_char* const file, const LJ_uint line, const LJ_char* const func )
{
	LJ_bool found = LJ_FALSE;
	const LJ_bool success = LJ_memFreeInternal( address );
	if ( success == LJ_FALSE )
	{
		LJ_outputPrintGold( ( "Error: unknown pointer 0x%X %s() %s:%d\n", address, func, file, line ) );
	}

	// Now free it from the debug tracking (leak detection list)
	found = LJ_memDebugRemoveAllocation( address );
	if ( found == LJ_FALSE )
	{
		// Error address isn't in the tracking list (output an error message)
		LJ_outputPrintGold( ( "\n" ) );
		LJ_outputPrintGold( ( "*********************************************\n" ) );
		LJ_outputPrintGold( ( "\n" ) );
		LJ_outputPrintGold( ( "Error: pointer 0x%X not found in the memory debug tracking list %s() %s:%d\n",
							  address, func, file, line ) );
		LJ_outputPrintGold( ( "\n" ) );
		LJ_outputPrintGold( ( "*********************************************\n" ) );
		LJ_outputPrintGold( ( "\n" ) );

		return LJ_FALSE;
	}
	return LJ_TRUE;
}

#endif // #if LJ_USE_MEM_TRACKING

void* LJ_memAllocInternal( LJ_memHeapHandle heapHandle, const LJ_uint elementSize, const LJ_uint numElements, LJ_uint* const allocSize )
{
	void* address = LJ_NULL;
	const int totalSize = elementSize * numElements;
	LJ_bool success = LJ_FALSE;

	// The special case of the system allocator
	if ( heapHandle == g_memSystemAllocHandle )
	{
		address = malloc( totalSize );
		if ( allocSize != LJ_NULL )
		{
			*allocSize = totalSize;
		}
	}
	// Find a free active pointer and add it to the list 
	success = LJ_memAllocAddPointer( address, heapHandle );
	if ( success == LJ_FALSE )
	{
		// Error message or an assert?
		return LJ_NULL;
	}

	return address;
}

// Returns LJ_FALSE if the address can't be free'd i.e. not a valid address e.g. a double free
LJ_bool LJ_memFreeInternal( void* const address )
{
	const LJ_memHeapHandle heapHandle = LJ_memAllocFreePointer( address );

	// Have to have a tracking list of valid ptrs to convert an address into its correct heap
	if ( heapHandle == LJ_MEM_HEAP_INVALID )
	{
		// So failed to free the ptr (output an error message)
		LJ_outputPrintGold( ( "\n" ) );
		LJ_outputPrintGold( ( "*********************************************\n" ) );
		LJ_outputPrintGold( ( "\n" ) );
		LJ_outputPrintGold( ( "Error: pointer 0x%X was not found in the active pointer list\n", address ) );
		LJ_outputPrintGold( ( "Either the pointer value is corrupt or the pointer has already been free'd\n" ) );
		LJ_outputPrintGold( ( "\n" ) );
		LJ_outputPrintGold( ( "*********************************************\n" ) );
		LJ_outputPrintGold( ( "\n" ) );
		return LJ_FALSE;
	}

	// The special case of the system allocator
	if ( heapHandle == g_memSystemAllocHandle )
	{
		free( address );
	}

	return LJ_TRUE;
}

