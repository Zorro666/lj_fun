#ifndef LJ_TYPES_H
#define LJ_TYPES_H

#include <stdarg.h>
#include <sys/types.h>

void LJ_typesInit( void );

///////////////////////////////////////////////////////////////////////////////////
//
// Base types
//
// These types should be per compiler/platform
//
///////////////////////////////////////////////////////////////////////////////////

// Native types use the native compiler/CPU sizes (for performance)
typedef int LJ_bool;

typedef char LJ_char;
typedef unsigned char LJ_uchar;

typedef signed short LJ_short;
typedef unsigned short LJ_ushort;

typedef signed int LJ_int;
typedef unsigned int LJ_uint;

typedef signed long long int LJ_long;
typedef unsigned long long int LJ_ulong;

typedef float LJ_float;
typedef double LJ_double;

#if PLATFORM_LINUX

// Types of fixed bit size e.g. for reading/writing to/from files
typedef int8_t LJ_char8;
typedef int16_t LJ_char16;

typedef int8_t LJ_int8;
typedef u_int8_t LJ_uint8;

typedef int16_t LJ_int16;
typedef u_int16_t LJ_uint16;

typedef int32_t LJ_int32;
typedef u_int32_t LJ_uint32;

typedef int64_t LJ_int64;
typedef u_int64_t LJ_uint64;

// TODO: need a 128-bit type

typedef float LJ_float32;
typedef double LJ_float64;

// VA args (yuck yuck) and very compiler/platform specific
typedef va_list LJ_valist;

// Setup variable argument list to point at the parameter following 'v'.
#define LJ_VA_START( ap, v )		( va_start( (ap), (LJ_int8*)(v) ) )

// Read the value of the argument in the variable argument list as the type specified by 't'.
#define LJ_VA_ARG( ap, t )		( va_arg( (ap), t ) )

// Call when finished reading variable argument list.
#define LJ_VA_END( ap )			( va_end( (ap) ) )

// Compiler dependent preprocessor macros
#define LJ_SOURCE_FILE 		__FILE__
#define LJ_SOURCE_LINE 		__LINE__
#define LJ_SOURCE_FUNCTION 	__PRETTY_FUNCTION__

#ifdef LJ_DEBUG
#define LJ_EXTERN_INLINE static inline
#else // #ifdef LJ_DEBUG
#define LJ_EXTERN_INLINE extern inline
#endif // #ifdef LJ_DEBUG

#endif // #if PLATFORM_LINUX

#if PLATFORM_PC

// Types of fixed bit size e.g. for reading/writing to/from files
typedef __int8 LJ_char8;
typedef __int16 LJ_char16;

typedef __int8 LJ_int8;
typedef unsigned __int8 LJ_uint8;

typedef __int16 LJ_int16;
typedef unsigned __int16 LJ_uint16;

typedef __int32 LJ_int32;
typedef unsigned __int32 LJ_uint32;

typedef __int64 LJ_int64;
typedef unsigned __int64 LJ_uint64;

// TODO: need a 128-bit type

typedef float LJ_float32;
typedef double LJ_float64;

// VA args (yuck yuck) and very compiler/platform specific
typedef char* LJ_valist;

#define __DO_NOT_USE_VA_SIZEOF(n)   ( (sizeof(n) + sizeof(LJ_int32) - 1) & ~(sizeof(LJ_int32) - 1) )

// Setup variable argument list to point at the parameter following 'v'.
#define LJ_VA_START(ap, v)	    ( ap = (LJ_valist)&v + __DO_NOT_USE_VA_SIZEOF(v) )

// Read the value of the argument in the variable argument list as the type specified by 't'.
#define LJ_VA_ARG(ap, t)	    ( *(t *)((ap += sizeof(t)) - __DO_NOT_USE_VA_SIZEOF(t)) )

// Call when finished reading variable argument list.
#define LJ_VA_END(ap)		    ( ap = (LJ_valist)0 )

// Compiler dependent preprocessor macros
#define LJ_SOURCE_FILE 		__FILE__
#define LJ_SOURCE_LINE 		__LINE__
#define LJ_SOURCE_FUNCTION 	__PRETTY_FUNCTION__

#define LJ_EXTERN_INLINE extern __inline

#pragma warning ( disable : 4116 )	// warning C4116: unnamed type definition in parentheses e.g. in the C compile time assert
#pragma warning ( disable : 4127 )	// warning C4127: conditional expression is constant e.g. while ( 1 )
#pragma warning ( disable : 4210 )	// warning C4210: nonstandard extension used : function given file scope e.g. the unittest register macros

#endif // #if PLATFORM_PC

// For the boolean types
#define LJ_TRUE 	(1)
#define LJ_FALSE 	(0)

#define LJ_NULL		(0)

// Handy sizes
#define LJ_1KB		(1024)
#define LJ_1MB		(1024*1024)

// Useful struct union to handle float/int pointer aliasing
typedef struct LJ_floatIntUnion
{
    union
    {
	LJ_float floatVal;

	LJ_uchar ucharVal[4];
	LJ_char charVal[4];

	LJ_ushort ushortVal[2];
	LJ_short shortVal[2];

	LJ_uint uintVal;
	LJ_int intVal;
    } data;
} LJ_floatIntUnion;

#endif // #ifndef LJ_TYPES_H

