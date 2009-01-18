#ifndef LJ_TYPESWIN_H
#define LJ_TYPESWIN_H

#include <sys/types.h>

///////////////////////////////////////////////////////////////////////////////////
//
// Base types
//
// These types are per compiler/platform
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
#define LJ_SOURCE_FUNCTION 	__FUNCTION__

#define LJ_EXTERN_INLINE extern __inline

#pragma warning ( disable : 4116 )	// warning C4116: unnamed type definition in parentheses e.g. in the C compile time assert
#pragma warning ( disable : 4127 )	// warning C4127: conditional expression is constant e.g. while ( 1 )
#pragma warning ( disable : 4210 )	// warning C4210: nonstandard extension used : function given file scope e.g. the unittest register macros

#endif // #ifndef LJ_TYPESWIN_H

