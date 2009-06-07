#ifndef LJ_TYPESLINUX_H
#define LJ_TYPESLINUX_H

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

#endif // #ifndef LJ_TYPESLINUX_H

