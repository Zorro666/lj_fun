#include "LJ.h"

#include "LJ_Base/LJ_assert.h"
#include "LJ_Base/LJ_output.h"
#include "LJ_Base/LJ_str.h"

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

extern LJ_int LJ_guiMessageBox( const LJ_int numButtons, const LJ_char* const buttonTexts[], 
						 		const LJ_char* const title, const LJ_char* const message );

LJ_int LJ_internalAssertDisplay( const LJ_char* const format, ... )
{
	const LJ_char* const buttonTexts[] = { "Continue", "Ignore", "Break" };
	LJ_char messageBuffer[1024];
	LJ_char messageBuffer2[512];
	LJ_valist args;
	LJ_int whichButton = -1;
	LJ_int debugBreak = 0;

	LJ_VA_START( args, format );
	LJ_strVSPrint( messageBuffer2, 512, format, &args );

	LJ_outputPrint( "******************************************************************************\n" );
	LJ_outputPrint( "\n" );
	LJ_outputPrint( "            ASSERT: assert(%s)\n", s_params.exprStr );
	LJ_outputPrint( "\n" );
	LJ_outputPrint( "%s:%d - %s\n", s_params.file, s_params.line, s_params.function );
	LJ_outputPrint( "\n" );
	LJ_outputPrint( "%s", messageBuffer2 );
	LJ_outputPrint( "\n" );
	LJ_outputPrint( "******************************************************************************\n" );

	messageBuffer[0] = '\0';
	LJ_strSPrint( messageBuffer, 1024, "%sASSERT: assert(%s)\n", messageBuffer, s_params.exprStr );
	LJ_strSPrint( messageBuffer, 1024, "%s\n", messageBuffer );
	LJ_strSPrint( messageBuffer, 1024, "%s%s:%d - %s\n", messageBuffer, s_params.file, s_params.line, s_params.function );
	LJ_strSPrint( messageBuffer, 1024, "%s\n%s", messageBuffer, messageBuffer2 );

	whichButton = LJ_guiMessageBox( 3, buttonTexts, "Assert", messageBuffer );

	switch ( whichButton )
	{
		case 0:
		{
			// Continue button
			debugBreak = 0;
			break;
		}
		case 1:
		{
			// Ignore button
			// Ignore future occurrences of this assert
			*(s_params.ignoreThisPtr) = 1;
			debugBreak = 0;
			break;
		}
		case 2:
		{
			// Break button
			debugBreak = 1;
			break;
		}
		default:
		{
			// Unknown
			debugBreak = 0;
			break;
		}
	}

	return debugBreak;
}

#endif // #if LJ_USE_ASSERT

