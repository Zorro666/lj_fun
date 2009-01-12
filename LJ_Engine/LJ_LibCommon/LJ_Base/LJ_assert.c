#include "LJ.h"
#include "LJ_assert.h"
#include "LJ_output.h"

#if LJ_USE_ASSERT

#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////////
//
// Runtime asserts in C
//
///////////////////////////////////////////////////////////////////////////////////

LJ_int LJ_internalAssert( LJ_int* const ignoreThisPtr,
						  const LJ_char* exprStr, 
						  const LJ_char* const file, const LJ_uint line, const LJ_char* const function,
						  const LJ_char* const format, ... 
						)
{
	LJ_valist args;
	LJ_VA_START( args, format );
	LJ_outputPrint( "******************************************************************************\n" );
	LJ_outputPrint( "\n" );
	LJ_outputPrint( "            ASSERT: assert(%s)\n", exprStr );
	LJ_outputPrint( "\n" );
	LJ_outputPrint( "%s:%d - %s\n", file, line, function );
	LJ_outputPrint( "\n" );
	LJ_outputPrint( format, &args );
	LJ_outputPrint( "\n" );
	LJ_outputPrint( "******************************************************************************\n" );

	// Ignore future occurrences of this assert
	*ignoreThisPtr = 1;

	// return 1 or 0 to cause a debug break
	return 1;
}

#endif // #if LJ_USE_ASSERT

