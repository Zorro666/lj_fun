#include "LJ.h"

#include "LJ_Math/LJ_math.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: Provides a suite of replacement functions for math.h functions and adds some other useful ones.
//
/////////////////////////////////////////////////////////////////////////////////////////////////

// Not an efficient implementation but pow shouldn't be used in time critical code!
// This function could easily be changed to take a float for its base value
LJ_float LJ_mathPowf( const LJ_int base, const LJ_int exponent )
{
	LJ_float value = 1.0f;
	LJ_int i;
	if ( exponent > 0 )
	{
		const LJ_float multiplier = (LJ_float)base;
		for ( i = 0; i < exponent; i++ )
		{
			value *= multiplier;
		}
	}
	else
	{
		const LJ_float multiplier = 1.0f / (LJ_float)base;
		for ( i = 0; i > exponent; i-- )
		{
			value *= multiplier;
		}
	}
	return value;
}

///////////////////////////////////////////////////////////////////
//
// Random number related functions
//
// This program implements the Mersenne twister algorithm for generation of pseudorandom numbers. 
// The program returns random integers in the range 0 to 2^32-1 (this holds even if a long int is
// larger than 32 bits). 
// 
///////////////////////////////////////////////////////////////////

#define LJ_MT_LEN 624

static LJ_int s_mt_index;
static LJ_uint s_mt_buffer[LJ_MT_LEN];

#define LJ_MT_IA           		397
#define LJ_MT_IB           		( LJ_MT_LEN - LJ_MT_IA )
#define LJ_UPPER_MASK      		0x80000000L
#define LJ_LOWER_MASK      		0x7FFFFFFFL
#define LJ_MATRIX_A        		0x9908B0DFL
#define LJ_TWIST( b, i, j )    	( (b)[i] & LJ_UPPER_MASK ) | ( (b)[j] & LJ_LOWER_MASK )
#define LJ_MAGIC( s )        	( ( (s) & 0x1 ) * LJ_MATRIX_A )

LJ_uint LJ_mathGetRand32( void )
{
    LJ_uint* const buffer = s_mt_buffer;
    LJ_int index = s_mt_index;
	LJ_uint value;
	
    if ( index == LJ_MT_LEN )
    {
    	LJ_uint s;
    	LJ_int i = 0;
        index = 0;
        for ( ; i < LJ_MT_IB; i++ ) 
		{
            s = LJ_TWIST( buffer, i, i+1 );
            buffer[i] = buffer[i + LJ_MT_IA] ^ ( s >> 1 ) ^ LJ_MAGIC( s );
        }
        for ( ; i < LJ_MT_LEN-1; i++ ) 
		{
            s = LJ_TWIST( buffer, i, i+1 );
            buffer[i] = buffer[i - LJ_MT_IB] ^ ( s >> 1 ) ^ LJ_MAGIC( s );
        }
        
        s = LJ_TWIST( buffer, LJ_MT_LEN-1, 0 );
        buffer[LJ_MT_LEN-1] = buffer[LJ_MT_IA-1] ^ ( s >> 1 ) ^ LJ_MAGIC( s );
    }
    s_mt_index = index + 1;

	// Note even though value is in a LJ_ulong the max is 2^32-1 e.g. 0xFFFFFFFF 
	value = (LJ_uint)( buffer[index] );

    return value;
}

// random int with value minVal-maxVal (including the min and max value)
LJ_int LJ_mathGetRandRange( const LJ_int minVal, const LJ_int maxVal )
{
	const LJ_uint value = LJ_mathGetRand32();

	// Convert to 0.0f -> 1.0f
	const LJ_float norm = (LJ_float)value / (LJ_float)( LJ_RAND_MAX );
	const LJ_float range = (LJ_float)( maxVal - minVal );
	const LJ_float rangeFloat = norm * range;

	// Convert 0.0f -> 1.0f into 0 -> range
	const LJ_int rangeValue = (LJ_int)( rangeFloat + 0.5f );

	// Convert 0 -> range to minVal -> maxVal
	const LJ_int randValue = minVal + rangeValue;

	return randValue;
}

// random int with value 0->maxVal (including 0 and the max value)
LJ_int LJ_mathGetRandMax( const LJ_int maxVal )
{
	return LJ_mathGetRandRange( 0, maxVal );
}

// random float with value minVal->maxVal (including the min and max value)
LJ_float LJ_mathGetRandRangeFloat( const LJ_float minVal, const LJ_float maxVal )
{
	const LJ_uint value = LJ_mathGetRand32();

	// Convert to 0.0f -> 1.0f
	const LJ_float norm = (LJ_float)value / (LJ_float)( LJ_RAND_MAX );
	const LJ_float range = ( maxVal - minVal );

	// Convert 0.0f -> 1.0f into 0.0f -> range
	const LJ_float rangeValue = norm * range;

	// Convert 0 -> range to minVal -> maxVal
	const LJ_float randValue = minVal + rangeValue;

	return randValue;
}

// random float with value 0.0f->maxVal (including 0.0f and the max value)
LJ_float LJ_mathGetRandMaxFloat( const LJ_float maxVal )
{
	return LJ_mathGetRandRangeFloat( 0.0f, maxVal );
}


// Seed the random number generator
void LJ_mathSeedRand( const LJ_uint seed )
{
    // Initialize generator state with seed
    // See Knuth TAOCP Vol 2, 3rd Ed, p.106 for multiplier.
    // In previous versions, most significant bits (MSBs) of the seed affect
    // only MSBs of the state array.  Modified 9 Jan 2002 by Makoto Matsumoto.
    LJ_uint* s = s_mt_buffer;
    LJ_uint* r = s_mt_buffer;
    LJ_int i = 1;
    *s = seed & 0xFFFFFFFFUL;
    *s++ = (LJ_uint)( (LJ_ulong)( 1812433253UL * ( *r ^ (*r >> 30) ) + i ) & 0xFFFFFFFFUL );
    for( ; i < LJ_MT_LEN; ++i )
    {
	*s++ = (LJ_uint)( (LJ_ulong)( 1812433253UL * ( *r ^ (*r >> 30) ) + i ) & 0xFFFFFFFFUL );
	r++;
    }
    s_mt_index = 0;
}

