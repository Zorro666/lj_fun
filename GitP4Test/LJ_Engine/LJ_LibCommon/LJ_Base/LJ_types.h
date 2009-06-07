#ifndef LJ_TYPES_H
#define LJ_TYPES_H

#include <stdarg.h>

void LJ_typesInit( void );

///////////////////////////////////////////////////////////////////////////////////
//
// Base types
//
// These types are per compiler/platform
//
///////////////////////////////////////////////////////////////////////////////////

#if LJ_PLATFORM_LINUX
#include "LJ_LINUX/LJ_typesLINUX.h"
#endif // #if PLATFORM_LINUX

#if LJ_PLATFORM_WIN
#include "LJ_WIN/LJ_typesWIN.h"
#endif // #if PLATFORM_WIN

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

// Default types min & max
#define LJ_TYPES_CHAR_MAX (+127)
#define LJ_TYPES_CHAR_MIN (-LJ_TYPES_CHAR_MAX-1)

#define LJ_TYPES_UCHAR_MAX (+255U)
#define LJ_TYPES_UCHAR_MIN (0U)

#define LJ_TYPES_SHORT_MAX (+32767)
#define LJ_TYPES_SHORT_MIN (-LJ_TYPES_SHORT_MAX-1)

#define LJ_TYPES_USHORT_MAX (+65535U)
#define LJ_TYPES_USHORT_MIN (0U)

#define LJ_TYPES_INT_MAX (+2147483647)
#define LJ_TYPES_INT_MIN (-LJ_TYPES_INT_MAX-1)

#define LJ_TYPES_UINT_MAX (+4294967295U)
#define LJ_TYPES_UINT_MIN (0U)

#define LJ_TYPES_FLOAT_MAX (+1.0e+37f)
#define LJ_TYPES_FLOAT_MIN (-LJ_TYPES_FLOAT_MAX)

#endif // #ifndef LJ_TYPES_H

