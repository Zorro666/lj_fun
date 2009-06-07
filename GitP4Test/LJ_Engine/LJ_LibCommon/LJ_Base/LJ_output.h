#ifndef LJ_OUTPUT_H
#define LJ_OUTPUT_H

// internal dependencies
#include "LJ_types.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: basic output functions e.g. print/console output
//
/////////////////////////////////////////////////////////////////////////////////////////////////

LJ_int LJ_outputVPrint( const LJ_char* const format, LJ_valist* list );
LJ_int LJ_outputPrint( const LJ_char* const format, ... );

#if LJ_NDEBUG
#define LJ_outputPrintDebug( format ) (void)(0)
#else // #if LJ_NDEBUG
#define LJ_outputPrintDebug( format ) LJ_outputPrint format 
#endif // #if LJ_NDEBUG

#if LJ_GOLD
#define LJ_outputPrintRelease( format ) (void)(0)
#else // #if LJ_GOLD
#define LJ_outputPrintRelease( format ) LJ_outputPrint format 
#endif // #if LJ_GOLD

#if LJ_MASTER
#define LJ_outputPrintGold( format ) (void)(0)
#else // #if LJ_MASTER
#define LJ_outputPrintGold( format ) LJ_outputPrint format
#endif // #if LJ_MASTER

#endif // #ifndef LJ_OUTPUT_H
