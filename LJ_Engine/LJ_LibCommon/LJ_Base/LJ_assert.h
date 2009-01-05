#ifndef LJ_ASSERT_H
#define LJ_ASSERT_H

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

