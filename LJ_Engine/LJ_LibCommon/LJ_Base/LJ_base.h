#ifndef LJ_BASE_HH
#define LJ_BASE_HH

///////////////////////////////////////////////////////////////////////////////////
//
// Base types
//
///////////////////////////////////////////////////////////////////////////////////

// Native types use the native compiler/CPU sizes (for performance)
typedef int LJ_bool;

typedef char LJ_char;
typedef unsigned char LJ_uchar;

typedef short LJ_short;
typedef unsigned short LJ_ushort;

typedef int LJ_int;
typedef unsigned int LJ_uint;

typedef float LJ_float;
typedef double LJ_double;

// Types of fixed bit size e.g. for reading/writing to/from files
typedef LJ_char LJ_int8;
typedef LJ_uchar LJ_uint8;

typedef LJ_short LJ_int16;
typedef LJ_ushort LJ_uint16;

typedef LJ_int LJ_int32;
typedef LJ_uint LJ_uint32;

typedef float LJ_float32;
typedef double LJ_float64;

// For the boolean types
#define LJ_TRUE 	(1)
#define LJ_FALSE 	(0)

// Handy sizes
#define LJ_1KB		(1024)
#define LJ_1MB		(1024*1024)

#endif // #ifndef LJ_BASE_HH

