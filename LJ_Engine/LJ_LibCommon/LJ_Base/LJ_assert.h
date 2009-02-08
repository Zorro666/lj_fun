#ifndef LJ_ASSERT_H
#define LJ_ASSERT_H

#include "LJ_types.h"

///////////////////////////////////////////////////////////////////////////////////
//
// Runtime asserts in C
//
///////////////////////////////////////////////////////////////////////////////////

#if LJ_USE_ASSERT

#ifndef LJ_DEBUGBREAK

#define LJ_DEBUGBREAK()		{ *((LJ_int*)LJ_NULL) = 0; }

#endif // #ifndef LJ_DEBUGBREAK

LJ_int LJ_internalAssertPrepare( LJ_int* const ignoreThisPtr,
						  	     const LJ_char* exprStr, 
						  		 const LJ_char* const file, const LJ_uint line, const LJ_char* const func
							   );

LJ_int LJ_internalAssertDisplay( const LJ_char* const format, ... );

// Because of the way the args work have to put extra brackets around the format arg
#define LJ_assert( expr, format ) \
	do \
	{ \
		if ( !(LJ_int)( (expr) ) ) \
		{ \
			static LJ_int __ignoreThisAssert = 0; \
			if ( __ignoreThisAssert == 0 ) \
			{ \
				if ( LJ_internalAssertPrepare( &__ignoreThisAssert, #expr, LJ_SOURCE_FILE, LJ_SOURCE_LINE, LJ_SOURCE_FUNCTION ) ) \
				{ \
					if ( LJ_internalAssertDisplay format ) \
					{ \
						LJ_DEBUGBREAK(); \
					} \
				} \
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

#define LJ_COMPILE_TIME_ASSERT_INTERNAL(expr) ( !!sizeof( struct { unsigned int LJ_VERIFY_ERROR_IF_NEGATIVE_SIZE__ : ( (expr) ? 1 : -1 ); } ) )

// For use in function scope
#define LJ_COMPILE_TIME_ASSERT(expr) (void)LJ_COMPILE_TIME_ASSERT_INTERNAL( expr )

// For use at global scope (outside of function scope)
#define LJ_COMPILE_TIME_ASSERT_GLOBAL(expr) extern int (* LJ_VERIFY_FUNCTION__ ( void ) ) [ (int)LJ_COMPILE_TIME_ASSERT_INTERNAL(expr) ]

#endif // #ifndef LJ_ASSERT_H

