#include "LJ.h"

#if LJ_USE_MEM_TRACKING

#include "LJ_Mem/LJ_memDebug.h"
#include "LJ_Base/LJ_output.h"

typedef struct LJ_memDebugInfo
{
	LJ_memHeapHandle heapHandle;
	LJ_uint elementSize;
	LJ_uint numElements;
	LJ_uint size;
	const LJ_char* file;
	LJ_uint line;
	const LJ_char* func;
	const LJ_char* type;
	const void* address;
} LJ_memDebugInfo;

static LJ_memDebugInfo s_memDebugInfos[LJ_MEM_ALLOC_MAX_NUM_ACTIVE_POINTERS];
static LJ_uint s_memDebugMaxActivePointer = 0;
static LJ_uint s_memDebugFirstFreePointer = 0;

void LJ_memDebugInit( void )
{
	LJ_uint i;
	for ( i = 0; i < LJ_MEM_ALLOC_MAX_NUM_ACTIVE_POINTERS; i++ )
	{
		LJ_memDebugResetAllocation( s_memDebugInfos+i );
	}
	s_memDebugMaxActivePointer = 0;
	s_memDebugFirstFreePointer = 0;
}

// Need to consider what this should do!
void LJ_memDebugReset( void )
{
}

// This loops over printing out any outstanding allocations
void LJ_memDebugShutdown( void )
{
	LJ_uint i;

	// Count the number of leaks first
	LJ_uint numLeaks = 0;
	LJ_uint totalBytesLeaked = 0;
	for ( i = 0; i <= s_memDebugMaxActivePointer; i++ )
	{
		LJ_memDebugInfo* const memDebugInfo = s_memDebugInfos + i;
		if ( memDebugInfo->address != LJ_NULL )
		{
			numLeaks++;
			totalBytesLeaked += memDebugInfo->size;
		}
	}

	// Display the leaks if there are any
	if ( numLeaks > 0 )
	{
		LJ_outputPrintGold( ( "\n" ) );
		LJ_outputPrintGold( ( "*********************************************\n" ) );
		LJ_outputPrintGold( ( "\n" ) );
		LJ_outputPrintGold( ( "ERROR: Memory Leaks\n" ) );
		LJ_outputPrintGold( ( "\n" ) );
		LJ_outputPrintGold( ( "%d bytes leaked in %d leak(s)\n", totalBytesLeaked, numLeaks ) );
		LJ_outputPrintGold( ( "\n" ) );
		LJ_outputPrintGold( ( "*********************************************\n" ) );
		LJ_outputPrintGold( ( "\n" ) );

		LJ_outputPrintGold( ( "Memory Leak details:\n" ) );
		LJ_outputPrintGold( ( "\n" ) );
		for ( i = 0; i <= s_memDebugMaxActivePointer; i++ )
		{
			LJ_memDebugInfo* const memDebugInfo = s_memDebugInfos + i;
			if ( memDebugInfo->address != LJ_NULL )
			{
				LJ_outputPrintGold( ( "Heap %d Ptr 0x%X Size:%d bytes ( %s[%d] ) %s() %s:%d \n",
								  	  memDebugInfo->heapHandle, 
									  memDebugInfo->address, memDebugInfo->size,
								  	  memDebugInfo->type, memDebugInfo->numElements, 
								  	  memDebugInfo->func, memDebugInfo->file, memDebugInfo->line ) );
			}
			// Clear it out
			LJ_memDebugResetAllocation( memDebugInfo );
		}
		LJ_outputPrintGold( ( "\n" ) );
		LJ_outputPrintGold( ( "*********************************************\n" ) );
		LJ_outputPrintGold( ( "\n" ) );
	}
}

// Add an allocation to the debug info list
LJ_bool LJ_memDebugAddAllocation( const void* address, const LJ_memHeapHandle heapHandle, 
		 						  const LJ_uint elementSize, const LJ_uint numElements, const LJ_uint size, 
			   					  const LJ_char* const file, const LJ_uint line, const LJ_char* const func, const LJ_char* const type )
{
	// Very slow linear array - perhaps one day make it a binary chop or linear sorted array
	LJ_uint i;
	for ( i = s_memDebugFirstFreePointer; i < LJ_MEM_ALLOC_MAX_NUM_ACTIVE_POINTERS; i++ )
	{
		LJ_memDebugInfo* const memDebugInfo = s_memDebugInfos + i;
		if ( memDebugInfo->address == LJ_NULL )
		{
			memDebugInfo->address = address;
			memDebugInfo->heapHandle = heapHandle;
			memDebugInfo->elementSize = elementSize;
			memDebugInfo->numElements = numElements;
			memDebugInfo->size = size;
			memDebugInfo->file = file;
			memDebugInfo->line = line;
			memDebugInfo->func = func;
			memDebugInfo->type = type;

			s_memDebugFirstFreePointer++;

			if ( i > s_memDebugMaxActivePointer )
			{
				s_memDebugMaxActivePointer = i;
			}
			return LJ_TRUE;
		}
	}
	return LJ_FALSE;
}

// Remove an allocation from the debug info list
LJ_bool LJ_memDebugRemoveAllocation( const void* const address )
{
	// Very slow linear array - perhaps one day make it a binary chop or linear sorted array
	LJ_uint i;
	for ( i = 0; i <= s_memDebugMaxActivePointer; i++ )
	{
		LJ_memDebugInfo* const memDebugInfo = s_memDebugInfos + i;
		if ( memDebugInfo->address == address )
		{
			LJ_memDebugResetAllocation( memDebugInfo );

			if ( i < s_memDebugFirstFreePointer )
			{
				s_memDebugFirstFreePointer = i;
			}
			if ( i == s_memDebugMaxActivePointer )
			{
				// This might not be the true maximum - could do a backwards search to find the first non-free alloc
				if ( s_memDebugMaxActivePointer > 0 )
				{
					s_memDebugMaxActivePointer--;
				}
			}
			return LJ_TRUE;
		}
	}
	return LJ_FALSE;
}

// Free the specified memory allocation
void LJ_memDebugResetAllocation( LJ_memDebugInfo* const memDebugInfo )
{
	memDebugInfo->heapHandle = LJ_MEM_HEAP_INVALID;
	memDebugInfo->elementSize = 0;
	memDebugInfo->numElements = 0;
	memDebugInfo->size = 0;
	memDebugInfo->file = LJ_NULL;
	memDebugInfo->line = 0;
	memDebugInfo->func = LJ_NULL;
	memDebugInfo->type = LJ_NULL;
	memDebugInfo->address = LJ_NULL;
}

#endif // #if LJ_USE_MEM_TRACKING

