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

#endif // #ifndef LJ_TYPES_H

