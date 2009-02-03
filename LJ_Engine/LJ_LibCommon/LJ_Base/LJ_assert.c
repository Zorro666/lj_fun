#include "LJ.h"

#include "LJ_Base/LJ_assert.h"
#include "LJ_Base/LJ_output.h"

#if LJ_USE_ASSERT

///////////////////////////////////////////////////////////////////////////////////
//
// Runtime asserts in C
//
///////////////////////////////////////////////////////////////////////////////////

typedef struct LJ_assertInternalParams
{
	LJ_int* ignoreThisPtr;
	const LJ_char* exprStr;
	const LJ_char* file;
	LJ_uint line;
	const LJ_char* function;
} LJ_assertInternalParams;

static LJ_assertInternalParams s_params;

LJ_int LJ_internalAssertPrepare( LJ_int* const ignoreThisPtr,
						  	     const LJ_char* const exprStr, 
						  	     const LJ_char* const file, const LJ_uint line, const LJ_char* const function
							   )
{
	s_params.ignoreThisPtr = ignoreThisPtr;
	s_params.exprStr = exprStr;
	s_params.file = file;
	s_params.line = line;
	s_params.function = function;

	return LJ_TRUE;
}

LJ_int LJ_internalAssertDisplay( const LJ_char* const format, ... )
{
	LJ_valist args;
	LJ_VA_START( args, format );
	LJ_outputPrint( "******************************************************************************\n" );
	LJ_outputPrint( "\n" );
	LJ_outputPrint( "            ASSERT: assert(%s)\n", s_params.exprStr );
	LJ_outputPrint( "\n" );
	LJ_outputPrint( "%s:%d - %s\n", s_params.file, s_params.line, s_params.function );
	LJ_outputPrint( "\n" );
	LJ_outputVPrint( format, &args );
	LJ_outputPrint( "\n" );
	LJ_outputPrint( "******************************************************************************\n" );

	// Ignore future occurrences of this assert
	*(s_params.ignoreThisPtr) = 1;

	// return 1 or 0 to cause a debug break
	return 1;
}

#endif // #if LJ_USE_ASSERT

