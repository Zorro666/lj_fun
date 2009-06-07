#ifndef LJ_MATH_H
#define LJ_MATH_H

// internal dependencies
#include "LJ_LibCommon/LJ_Base/LJ_types.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: Provides a suite of replacement functions for math.h functions and adds some other useful ones.
//
/////////////////////////////////////////////////////////////////////////////////////////////////

// Not an efficient implementation but pow shouldn't be used in time critical code!
LJ_float LJ_mathPowf( const LJ_int base, const LJ_int exponent );

LJ_EXTERN_INLINE LJ_float LJ_mathFabsf( const LJ_float value );

// Random related functions
#define LJ_RAND_MAX 0xFFFFFFFF

// random number (32 bit) - all other Rand functions call this
LJ_uint LJ_mathGetRand32( void );

// random int with value minVal-maxVal (including the min and max value)
LJ_int LJ_mathGetRandRange( const LJ_int minVal, const LJ_int maxVal );

// random int with value 0->maxVal (including 0 and the max value)
LJ_int LJ_mathGetRandMax( const LJ_int maxVal );

// random float with value minVal->maxVal (including the min and max value)
LJ_float LJ_mathGetRandRangeFloat( const LJ_float minVal, const LJ_float maxVal );

// random float with value 0.0f->maxVal (including 0.0f and the max value)
LJ_float LJ_mathGetRandMaxFloat( const LJ_float maxVal );

// Seed the random number generator
void LJ_mathSeedRand( const LJ_uint seed );

// Inlines
LJ_EXTERN_INLINE LJ_float LJ_mathFabsf( const LJ_float value )
{
	// Could do this with bit shifting but that is slow if value is in a FPU register
	// It should be an instruction on most instruction sets
	const LJ_float result = ( value > 0.0f ) ? value : -value;
	return result;
}

#endif // #ifndef LJ_MATH_H
