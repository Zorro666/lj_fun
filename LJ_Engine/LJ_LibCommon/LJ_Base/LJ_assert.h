#ifndef LJ_ASSERT_H
#define LJ_ASSERT_H

#include "LJ_types.h"

///////////////////////////////////////////////////////////////////////////////////
//
// Runtime asserts in C
//
///////////////////////////////////////////////////////////////////////////////////

#if LJ_USE_ASSERT

LJ_int LJ_internalAssert( 
						  const LJ_char* exprStr, 
						  const LJ_char* const file, const LJ_uint line, const LJ_char* const func,
						  const LJ_char* const format, ... 
						);

#define LJ_assert( expr, format ) \
	do \
	{ \
		if ( !(int)( expr ) ) \
		{ \
			static LJ_uint8 __ignoreThis = 0; \
			if ( __ignoreThis == 0 ) \
			{ \
				__ignoreThis = LJ_internalAssert( #expr, LJ_SOURCE_FILE, LJ_SOURCE_LINE, LJ_SOURCE_FUNCTION, format ); \
			} \
		} \
	} while ( 0 )

#else // #if LJ_USE_ASSERT

#define LJ_assert( expr, format ) (void)(0)

#endif // #if LJ_USE_ASSERT

///////////////////////////////////////////////////////////////////////////////////
//
// Compile time asserts in C
//
///////////////////////////////////////////////////////////////////////////////////

// For use in function scope
#define LJ_COMPILE_TIME_ASSERT(expr) (void)( !!sizeof( struct { unsigned int LJ_VERIFY_ERROR_IF_NEGATIVE_SIZE__ : ( (expr) ? 1 : -1 ); } ) )

// For use at global scope (outside of function scope)
#define LJ_COMPILE_TIME_ASSERT_GLOBAL(expr) extern int ( *LJ_VERIFY_FUNCTION__( void ) ) [ LJ_COMPILE_TIME_ASSERT(expr) ]

#endif // #ifndef LJ_ASSERT_H

