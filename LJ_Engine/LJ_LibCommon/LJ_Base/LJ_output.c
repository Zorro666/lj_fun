#include "LJ.h"

#include "LJ_output.h"
#include "LJ_str.h"

#include <stdio.h>

#define LJ_OUTPUT_MAX_PRINT_STRING 4096
//
/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: basic output functions e.g. print/console output
//
/////////////////////////////////////////////////////////////////////////////////////////////////

LJ_int LJ_outputVPrint( const LJ_char* const format, LJ_valist* list )
{
	char outputString[LJ_OUTPUT_MAX_PRINT_STRING];
	const LJ_int numChars = LJ_strVSPrint( outputString, LJ_OUTPUT_MAX_PRINT_STRING, format, list );
	printf( "%s", outputString );

	return numChars;
}

LJ_int LJ_outputPrint( const LJ_char* const format, ... )
{
	LJ_valist args;
	LJ_VA_START( args, format );
	return LJ_outputVPrint( format, &args );
}

