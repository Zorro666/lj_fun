#ifndef LJ_TYPES_H
#define LJ_TYPES_H

#include <stdarg.h>
#include <sys/types.h>

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

typedef signed long int LJ_long;
typedef unsigned long int LJ_ulong;

typedef float LJ_float;
typedef double LJ_double;

// Types of fixed bit size e.g. for reading/writing to/from files
// These should be changed to use the explcit sizes
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

typedef float LJ_float32;
typedef double LJ_float64;

// VA args (yuck yuck)
typedef va_list LJ_valist;

// For the boolean types
#define LJ_TRUE 	(1)
#define LJ_FALSE 	(0)

#define LJ_NULL		(0)

// Handy sizes
#define LJ_1KB		(1024)
#define LJ_1MB		(1024*1024)

#endif // #ifndef LJ_TYPES_H

