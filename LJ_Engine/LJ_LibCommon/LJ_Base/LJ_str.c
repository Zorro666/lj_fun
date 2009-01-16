#include "LJ.h"

#include "LJ_str.h"
#include "LJ_output.h"
#include "LJ_assert.h"

// For atof
#include <stdlib.h>

//*******************************************************************
// Constants
//*******************************************************************

// Set of constants related to LJ_float to string conversion in LJ_strAppendFloat
#define MANTISSA_SHIFT		(23)
#define FIXED_POINT_SHIFT	(31)

#define START_DIGIT			(+8)
#define MAX_DIGIT			(-7)
#define END_DIGIT			(-3)

////////////////////////////////////////////////////////////////////////
//
// Struct:      FormatFlags
//
// Description: Flags used by replacement printf functions to store format specifiers for each field.
//
////////////////////////////////////////////////////////////////////////

typedef struct LJ_strFormatFlags
{
	LJ_int width;
	LJ_int precision;
	LJ_bool longSize;
	LJ_bool leftAlign;
	LJ_bool signPrefix;
	LJ_bool zeroPrefix;
} LJ_strFormatFlags;

static void LJ_strFormatFlagsReset( LJ_strFormatFlags* const formatFlags )
{
	formatFlags->width = -1;
	formatFlags->precision = -1;
	formatFlags->leftAlign = LJ_FALSE;
	formatFlags->signPrefix = LJ_FALSE;
	formatFlags->zeroPrefix = LJ_FALSE;
	formatFlags->longSize = LJ_FALSE;
}

static LJ_uint LJ_strAppendChars( LJ_char* const to, const LJ_uint currentLength, const LJ_uint maxLen, 
								  const LJ_int number, const LJ_char c )
{
	const LJ_uint maxLength = maxLen - 1;
	LJ_uint index = currentLength;
	LJ_int count;
	for ( count = 0; count < number; count++ )
	{
		if ( index == maxLength )
		{
			// string too long to fit; terminate here
			if ( to != LJ_NULL )
			{
				to[maxLength] = 0;
			}
			return maxLength;
		}

		if ( to != LJ_NULL )
		{
			to[index] = c;
		}
		index++;
	}

	return index;
}

static LJ_uint LJ_strAppendString( LJ_char* const to, const LJ_uint currentLength, const LJ_uint maxLen, 
								   const LJ_char* const append, const LJ_uint length, const LJ_strFormatFlags* const flags )
{
	const LJ_uint maxLength = maxLen - 1;
	LJ_uint index = currentLength;

	LJ_uint count = 0;

	// deal with width specification
	const LJ_int width = flags->width - length;
 	if ( width > 0 )
	{
		if ( flags->zeroPrefix == LJ_TRUE )
		{
			if ( append[0] == '-' || append[0] == '+' )
			{
				// need to move sign to start of prepended zeros
				index = LJ_strAppendChars( to, index, maxLen, 1, append[0] );
				count++;
			}

			index = LJ_strAppendChars( to, index, maxLen, width, '0' );
		}
		else if ( flags->leftAlign == LJ_FALSE )
		{
			index = LJ_strAppendChars( to, index, maxLen, width, ' ' );
		}
	}

	// put output buffer onto end of string

	for ( ; count < length; count++ )
	{
		if ( index == maxLength )
		{
			// string too long to fit; terminate here
			to[maxLength] = 0;
			return maxLength;
		}

		if ( to != LJ_NULL )
		{
			to[index] = append[count];
		}
		index++;
	}

	// pad with spaces if required
	if ( width > 0 && flags->leftAlign == LJ_TRUE )
	{
		index = LJ_strAppendChars( to, index, maxLength, width, ' ' );
	}

	return index;
}

static LJ_uint LJ_strAppendNumber( LJ_char* const to, const LJ_uint currentLength, const LJ_uint maxLen, 
							 	   const LJ_long value, const LJ_strFormatFlags* const flags, const int base )
{
	// temporary output buffer
	LJ_char output[32];
	LJ_uint index = 0;

	LJ_ulong mantissa;

	if ( value < 0 )
	{
		// -ve number; make +ve and output sign
		mantissa = -value;
		output[index++] = '-';
	}
	else
	{
		// +ve number
		mantissa = value;

		if ( flags->signPrefix == LJ_TRUE )
		{
			// output + sign if required
			output[index++] = '+';
		}
	}
	if ( base == 8 )
	{
		output[index++] = '0';
	}

	if ( mantissa == 0 )
	{
		// zero is special case
		output[index++] = '0';
	}
	else
	{
		// set divider to maximum digit possible
		//LJ_ulong divider = 1000000000000000000ULL;
		LJ_ulong divider = ( base == 10 ) ? 1000000000UL : 1073741824UL;

		// find first digit
		while ( divider > mantissa )
		{
			divider /= base;
		}

		// output digits
		while ( divider > 0 )
		{
			LJ_ulong number = mantissa / divider;

			output[index++] = '0' + (LJ_char)number;

			mantissa -= ( number * divider );
			divider /= base;
		}
	}

	// put this buffer onto end of string
	return LJ_strAppendString( to, currentLength, maxLen, output, index, flags );
}

static LJ_uint LJ_strAppendHex( LJ_char* const to, const LJ_uint currentLength, const LJ_uint maxLen, 
								const LJ_ulong value, const LJ_strFormatFlags* const flags )
{
	// temporary output buffer
	LJ_char output[16];
	LJ_uint index = 0;
	LJ_int digit;

	// default is 8 digit -> equivalent of %08X for 32-bit int's
	// default is 16 digit -> equivalent of %016X for 64-bit int's
	const LJ_int maxDigit = ( flags->longSize == LJ_FALSE ) ? 7 : 15;
	LJ_bool doOutput = ( flags->width < 0 ) ? LJ_TRUE : LJ_FALSE;
	for ( digit = maxDigit; digit >= 0; digit-- )
	{
		const LJ_ulong number = (value >> (digit * 4)) & 0xF;

		// look for the first non-zero character to start doing output (unless doing default output)
		if ( ( number > 0 ) || ( doOutput == LJ_TRUE ) )
		{
			output[index++] = ( number < 10 ) ? '0' + (LJ_char)number : 'A' + (LJ_char)( number - 10 );
			doOutput = LJ_TRUE;
		}
	}

	if ( doOutput == LJ_FALSE )
	{
		// if no output, then must be an unpadded zero
		output[index++] = '0';
	}

	// put this buffer onto end of string
	return LJ_strAppendString( to, currentLength, maxLen, output, index, flags );
}

static LJ_uint LJ_strAppendFloat( LJ_char* const to, const LJ_uint currentLength, const LJ_uint maxLen, 
								  const LJ_float value, const LJ_strFormatFlags* const flags )
{
	const LJ_ulong DECIMAL_POINT = ( 1UL << FIXED_POINT_SHIFT );
	const LJ_ulong DIV_DECIMAL = (LJ_ulong)( DECIMAL_POINT * 10000000UL );
	const LJ_ulong MIN_DECIMAL 	= ( DECIMAL_POINT / 10000000UL );
	const LJ_ulong MAX_DECIMAL 	= ( DIV_DECIMAL * 10UL );

	LJ_int endDigit;
	LJ_ulong decimal;
	LJ_int digitIndex;

	LJ_char output[32];
	LJ_uint index = 0;
	LJ_int digit;

	LJ_uint sign;
	LJ_int exponent;
	LJ_ulong mantissa;

	// get LJ_float as its binary representation
	LJ_floatIntUnion noAliasingTemp;
	LJ_uint binary;

	noAliasingTemp.data.floatVal = value;
	binary = noAliasingTemp.data.uintVal;

	// extract component parts
	sign = ( binary & 0x80000000 );
	exponent = ( ( binary - sign ) >> 23 ) - 127;
	mantissa = ( binary & 0x007FFFFFL ) + 0x00800000L;


	// turn into 33.31 fixed point number range which covers most numbers we'll want to deal with, right?
	exponent += ( FIXED_POINT_SHIFT - MANTISSA_SHIFT );

	// shift the mantissa into the required fixed point format
	if ( exponent < 0 )
	{
		mantissa >>= -exponent;
	}
	else if ( exponent > 0 )
	{
		mantissa <<= exponent;
	}

	// now output each number as we find it
	endDigit = ( flags->precision < 0 )? END_DIGIT : -flags->precision;
	if ( endDigit < MAX_DIGIT )
	{
		endDigit = MAX_DIGIT;
	}

	// add a little bit so get correct rounding on smallest decimal place
	decimal = ( MIN_DECIMAL / 2 );
	for ( digitIndex = MAX_DIGIT; digitIndex < endDigit; digitIndex++ )
	{
		decimal *= 10;
	}
	mantissa += decimal;

	// ready to start outputting string
	if ( sign != 0 )
	{
		// always output sign if -ve
		output[index++] = '-';
	}
	else if ( flags->signPrefix == LJ_TRUE )
	{
		// only do +ve if required
		output[index++] = '+';
	}

	// check the exponent and mantissa are still in range
	if ( ( exponent >= ( 64 - MANTISSA_SHIFT ) ) || ( mantissa >= MAX_DECIMAL ) )
	{
		// overflow
		LJ_strCopy( &output[index], "HUGE" );
		return LJ_strAppendString( to, currentLength, maxLen, output, index + 4, flags );
	}
	else if ( ( exponent < -MANTISSA_SHIFT ) || ( mantissa < MIN_DECIMAL ) )
	{
		// underflow
		const LJ_uint length = 2 - endDigit;
		LJ_strNCopy( &output[index], "0.0000000", length + 1 );
		return LJ_strAppendString( to, currentLength, maxLen, output, index + length, flags );
	}

	digit = START_DIGIT;

	// don't output anything till we find the first number, or decimal point
	while ( mantissa < DIV_DECIMAL )
	{
		mantissa *= 10;

		digit--;
		if (digit == 1)
		{
			// quit out if reach decimal point
			break;
		}
	}

	while ( digit > endDigit )
	{
		const LJ_uint number = (LJ_uint)( mantissa / DIV_DECIMAL );

		// output number
		output[index++] = '0' + (LJ_char)number;

		// NOTE: method keeps multiplying up mantissa to get next digit
		mantissa -= number * DIV_DECIMAL;
		mantissa *= 10;

		if ( digit == 1 )
		{
			// output decimal point when appropriate
			output[index++] = '.';
		}

		digit--;
	}

	// put this buffer onto end of string
	return LJ_strAppendString( to, currentLength, maxLen, output, index, flags );
}

static LJ_uint LJ_strAppendBinary( LJ_char* const to, const LJ_uint currentLength, const LJ_uint maxLen, 
								   const LJ_uint value, const LJ_strFormatFlags* const flags )
{
	// temporary output buffer
	LJ_char output[33];
	LJ_uint index = 0;
	LJ_int digit;

	// default is 32 digit -> equivalent of %032b
	LJ_bool doOutput = ( flags->width < 0 ) ? LJ_TRUE : LJ_FALSE;
	for ( digit = 31; digit >= 0; digit-- )
	{
		const LJ_uint bit = ( value >> digit ) & 0x1;

		// look for the first non-zero character to start doing output (unless doing default output)
		if ( bit != 0 || doOutput == LJ_TRUE )
		{
			output[index++] = (LJ_char)( '0' + bit );
			doOutput = LJ_TRUE;
		}
	}

	// put this buffer onto end of string
	return LJ_strAppendString( to, currentLength, maxLen, output, index, flags );
}

LJ_int LJ_strVSPrint( LJ_char* const to, const LJ_uint maxLen, const LJ_char* const format, LJ_valist* list )
{
	// allow space for terminator
	const LJ_uint maxLength = maxLen - 1;
	// get some flags regarding the formatting of numbers
	LJ_strFormatFlags flags;
	LJ_uint length = 0;
	LJ_uint index;

	// parsing mode
	typedef enum FCMode
	{
		FCM_NORMAL,
		FCM_FLAG,
		FCM_WIDTH,
		FCM_PRECISION,
		FCM_SIZE,
		FCM_FORMAT
	} FCMode;

	FCMode mode = FCM_NORMAL;

	if ( maxLen == 0 )
	{
		return 0;
	}


	LJ_strFormatFlagsReset( &flags );

	// parse through format string until reach terminator, or run out of buffer space
	for ( index = 0; length < maxLength; index++ )
	{
		const LJ_char chr = format[index];
		if ( chr == 0 )
		{
			break;
		}

		if ( mode == FCM_NORMAL )
		{
			if ( chr == '%' )
			{
				//percent symbol is start of format specification
				mode = FCM_FLAG;
				continue;
			}
			// anything else just gets copied normally
			if ( to != LJ_NULL )
			{
				to[length] = chr;
			}
			length++;
			continue;
		}

		if ( mode == FCM_FLAG )
		{
			switch (chr)
			{
				case '-':
				{
					flags.leftAlign = LJ_TRUE;
					continue;
				}

				case '+':
				{
					flags.signPrefix = LJ_TRUE;
					continue;
				}

				case '0':
				{
					// NOTE: '-' overrides '0' specification
					if (flags.leftAlign == LJ_FALSE)
					{
						flags.zeroPrefix = LJ_TRUE;
					}
					continue;
				}

				case ' ':
				case '#':
				{
					// not supported at present as never seen anyone use these
					continue;
				}

				default:
				{
					mode = FCM_WIDTH;
				}
			}
		}

		if ( mode == FCM_WIDTH )
		{
			// read width specification
			if ( chr >= '0' && chr <= '9' )
			{
				if ( flags.width < 0 )
				{
					// default value is -1, so just replace
					flags.width = chr - '0';
				}
				else
				{
					flags.width *= 10;
					flags.width += chr - '0';
				}
				continue;
			}

			if ( chr == '.' )
			{
				// dot marks the start of a precision field
				mode = FCM_PRECISION;
				continue;
			}

			mode = FCM_SIZE;
		}

		if ( mode == FCM_PRECISION )
		{
			// read the precision specification
			if ( chr >= '0' && chr <= '9' )
			{
				if ( flags.precision < 0 )
				{
					flags.precision = chr - '0';
				}
				else
				{
					flags.precision *= 10;
					flags.precision += chr - '0';
				}
				continue;
			}

			if ( chr == '*' )
			{
				// get precision from parameter
				flags.precision = LJ_VA_ARG( *list, LJ_int );
				mode = FCM_SIZE;
				continue;
			}

			mode = FCM_SIZE;
		}

		if ( mode == FCM_SIZE )
		{
			flags.longSize = LJ_FALSE;
			// size specifier - only 'l' is supported at the moment
			switch ( chr )
			{
				case 'l':
				{
					// means long input e.g. int64 or wide character or wide string
					// only int64 is supported at the moment
					flags.longSize = LJ_TRUE;
					mode = FCM_FORMAT;
					continue;
					break;
				}
			}
			mode = FCM_FORMAT;
		}
		
		if ( mode == FCM_FORMAT )
		{
			// interpret format character
			switch ( chr )
			{
				case 'c':
				{
					// character
					const LJ_char character = (LJ_char)LJ_VA_ARG( *list, LJ_int );
					if ( to != LJ_NULL )
					{
						to[length] = character;
					}
					length++;
					break;
				}

				case 's':
				{
					// string
					const LJ_char* subString = LJ_VA_ARG( *list, LJ_char* );
					LJ_int strLen;
					if ( subString == LJ_NULL )
					{
						// deal with LJ_NULL pointer
						subString = "(null)";
					}
						
					strLen = LJ_strGetLength( subString );
					if ( ( flags.precision > 0 ) && ( strLen > flags.precision ) )
					{
						// precision specifier can truncate strings
						strLen = flags.precision;
					}

					length = LJ_strAppendString( to, length, maxLength, subString, strLen, &flags );
					break;
				}

				case 'f':
				case 'g':
				case 'G':
				case 'e':
				case 'E':
				{
					const LJ_float value = (LJ_float)LJ_VA_ARG( *list, LJ_double );
					length = LJ_strAppendFloat( to, length, maxLength, value, &flags );
					break;
				}

				case 'o':
					{
						// octal integer
						const LJ_long value = ( flags.longSize == LJ_FALSE ) ? LJ_VA_ARG( *list, LJ_int ) : LJ_VA_ARG( *list, LJ_long );
						length = LJ_strAppendNumber( to, length, maxLength, value, &flags, 8 );
						break;
					}
				case 'd':
				case 'i':
				case 'u':
					{
						// decimal integer
						const LJ_long value = ( flags.longSize == LJ_FALSE ) ? LJ_VA_ARG( *list, LJ_int ) : LJ_VA_ARG( *list, LJ_long );
						length = LJ_strAppendNumber( to, length, maxLength, value, &flags, 10 );
						break;
					}

				case 'p':
				case 'x':
				case 'X':
					{
						// hex integer
						const LJ_ulong value = ( flags.longSize == LJ_FALSE ) ? LJ_VA_ARG( *list, LJ_uint ) : LJ_VA_ARG( *list, LJ_ulong );
						length = LJ_strAppendHex( to, length, maxLength, value, &flags );
						break;
					}

				case 'b':
					{
						// binary
						const LJ_uint value = LJ_VA_ARG( *list, LJ_uint );
						length = LJ_strAppendBinary( to, length, maxLength, value, &flags );
						break;
					}

				default:
					{
						// unknown character which just gets output
						if ( to != LJ_NULL )
						{
							to[length] = chr;
						}
						length++;
					}
			}

			// go back to normal string parsing
			mode = FCM_NORMAL;
			LJ_strFormatFlagsReset( &flags );
		}
	}

	// terminate the string
	if ( ( to != LJ_NULL ) && ( maxLength > 0 ) )
	{
		to[length] = 0;
	}
	return length;
}

LJ_int LJ_strSPrint( LJ_char* const to, const LJ_uint maxLength, const LJ_char* const format, ... )
{
	LJ_valist args;
	LJ_VA_START( args, format );
	return LJ_strVSPrint( to, maxLength, format, &args );
}

LJ_uint LJ_strGetLength( const LJ_char* const string )
{
	const LJ_char* read = string;
	LJ_uint length = 0;

	while ( *read != 0 )
	{
		read++;
		length++;
	}

	return length;
}

void LJ_strCopy( LJ_char* const to, const LJ_char* const from )
{
	const LJ_char* read = from;
	LJ_char* write = to;

	while (*read != 0)
	{
		*write = *read;
		read++;
		write++;
	}

	*write = 0;
}

void LJ_strNCopy( LJ_char* const to, const LJ_char* const from, const LJ_uint maxLength )
{
	LJ_uint index = 0;
	while ( index < maxLength - 1 )
	{
		to[index] = from[index];

		if ( from[index] == 0 )
		{
			// found terminator
			return;
		}

		index++;
	}

	// got to end of string - zero terminate
	to[index] = 0;
}

LJ_bool LJ_strIsSame( const LJ_char* const string, const LJ_char* const compare )
{
	const LJ_char* readString = string;
	const LJ_char* readCompare = compare;

	while ( *readString == *readCompare )
	{
		if ( *readString == 0 )
		{
			return LJ_TRUE;
		}

		readString++;
		readCompare++;
	}

	return LJ_FALSE;
}

LJ_bool LJ_strIsNSame( const LJ_char* const string, const LJ_char* const compare, const LJ_uint maxLength )
{
	const LJ_char* readString = string;
	const LJ_char* readCompare = compare;
	LJ_uint length = 0;

	if ( maxLength == 0 )
	{
		return LJ_TRUE;
	}

	while ( 1 )
	{
		if ( length == maxLength )
		{
			return LJ_TRUE;
		}
		else if ( *readString != *readCompare )
		{
			return LJ_FALSE;
		}

		if ( *readString == 0 )
		{
			return LJ_TRUE;
		}

		readString++;
		readCompare++;
		length++;
	}

	//  return LJ_FALSE;
}

LJ_int LJ_strFindChar( const LJ_char* const string, const LJ_char c )
{
	const LJ_char* read = string;

	while ( *read != 0 )
	{
		if ( *read == c )
		{
			return ( read - string );
		}

		read++;
	}

	return -1;
}

LJ_int LJ_strReverseFindChar( const LJ_char* const string, const LJ_char c )
{
	const LJ_char* read;

	// Catch the zero length string
	const LJ_int length = LJ_strGetLength( string );
	if ( length == 0 )
	{
		return -1;
	}

	read = string + length - 1;

	while ( *read != 0 )
	{
		if ( *read == c )
		{
			return ( read - string );
		}

		read--;
	}

	return -1;
}

void LJ_strCat( LJ_char* const to, const LJ_char* const from )
{
	LJ_char* write = to + LJ_strGetLength( to );
	LJ_strCopy( write, from );
}

void LJ_strCatChar( LJ_char* const to, const LJ_char c )
{
	LJ_char* write = to + LJ_strGetLength( to );
	*write = c;
	*(write + 1) = 0;
}

void LJ_strNCat( LJ_char* const to, const LJ_char* const from, const LJ_uint maxLength )
{
	LJ_uint length = LJ_strGetLength( to );

	LJ_strNCopy( &to[length], from, maxLength - length );
}

LJ_float LJ_strToFloat( const LJ_char* const string )
{
	// Algorithm is:
	// 1. eat white space
	// 2. + or -
	// 3. digits 
	// 4. optional - decimal point 
	// 5. digits
	// 6. optional - e/E : +/- : digits
	// 7. optional - f
	return (LJ_float)( atof( string ) );
}

LJ_int LJ_strToInt( const LJ_char* const string )
{
	// Algorithm is:
	// 1. eat white space
	// 2. + or -
	// 3. 0x -> means hex
	// 4. 0 -> means octal
	// 5. otherwise decimal
	// 6. convert string -> number of correct base until a non-valid digit is found

	LJ_int value = 0;
	LJ_int base = 0;
	LJ_int maxDigit;
	LJ_int maxAlpha;
	LJ_int negMult = +1;
	LJ_bool validChar = LJ_FALSE;

	// 1. eat white space
	const LJ_char* stringBuffer = LJ_strEatWhiteSpace( string );

	// 2. + or -
	if ( stringBuffer[0] == '-' )
	{
		negMult = -1;
		stringBuffer++;
	}
	else if ( stringBuffer[0] == '+' )
	{
		negMult = +1;
		stringBuffer++;
	}

	// 3. 0x -> means hex
	if ( ( stringBuffer[0] == '0' ) && ( stringBuffer[1] == 'x' ) )
	{
		base = 16;
		stringBuffer += 2;
	}
	// 4. 0 -> means octal
	else if ( stringBuffer[0] == '0' )
	{
		base = 8;
		stringBuffer++;
	}
	// 5. otherwise decimal
	else
	{
		base = 10;
	}

	maxDigit = ( base > 9 ) ? 9 : base;
	maxAlpha = ( base < 11 ) ? -1 : ( base - 11 );

	// 6. convert string -> number of correct base until a non-valid digit is found
	do 
	{
		int charValue = -1;
		const LJ_char c = stringBuffer[0];
		// '0' -> '9'
		int digitValue = c - '0';
		// 'A' -> 'Z'
		int alphaValue = LJ_strToUpperChar( c ) - 'A';

		validChar = LJ_FALSE;
		if ( ( digitValue < 0 ) || ( digitValue > maxDigit ) )
		{
			digitValue = -1;
		}
		if ( ( alphaValue < 0 ) || ( alphaValue > maxAlpha ) )
		{
			alphaValue = -1;
		}
		if ( ( digitValue >= 0 ) || ( alphaValue >= 0 ) )
		{
			charValue = ( digitValue >= 0 ) ? digitValue : ( alphaValue + 10 );
		}

		if ( ( charValue >= 0 ) && ( charValue < base ) )
		{
			value *= base;
			value += charValue;
			validChar = LJ_TRUE;
			stringBuffer++;
		}
	} while ( validChar == LJ_TRUE );

	value *= negMult;
	return value;
}

LJ_bool LJ_strIsSameIgnoreCase( const LJ_char* const string, const LJ_char* const compare )
{
	const LJ_char* readString = string;
	const LJ_char* readCompare = compare;

	while ( LJ_strToUpperChar( *readString ) == LJ_strToUpperChar( *readCompare ) )
	{
		if ( *readString == 0 )
		{
			return LJ_TRUE;
		}

		readString++;
		readCompare++;
	}

	return LJ_FALSE;
}

LJ_bool LJ_strIsNSameIgnoreCase( const LJ_char* const string, const LJ_char* const compare, const LJ_uint maxLength )
{
	const LJ_char* readString = string;
	const LJ_char* readCompare = compare;
	LJ_uint length = 0;

	LJ_char char1;
	do
	{
		const LJ_char char2 = LJ_strToUpperChar( *readCompare );
		char1 = LJ_strToUpperChar( *readString );

		if ( char1 != char2 )
		{
			return LJ_FALSE;
		}

		readString++;
		readCompare++;
		length++;

		if ( length == maxLength )
		{
			return LJ_TRUE;
		}
	}
	while ( char1 != 0 );

	return LJ_TRUE;
}

LJ_char LJ_strToUpperChar( const LJ_char c )
{
	LJ_char result = c;

	if ( ( c >= 'a') && ( c <= 'z') )
	{
		result -= ('a' -'A');
	}

	return result;
}

LJ_char LJ_strToLowerChar( const LJ_char c )
{
	LJ_char result = c;

	if ( ( c >= 'A') && ( c <= 'Z') )
	{
		result += ('a' -'A');
	}

	return result;
}

void LJ_strToUpper( LJ_char* const to, const LJ_char* const from )
{
	const LJ_char* read = from;
	LJ_char* write = to;

	while ( *read != 0 )
	{
		*write = LJ_strToUpperChar( *read );
		read++;
		write++;
	}

	*write = 0;
}

void LJ_strToLower( LJ_char* const to, const LJ_char* const from )
{
	const LJ_char* read = from;
	LJ_char* write = to;

	while ( *read != 0 )
	{
		*write = LJ_strToLowerChar( *read );
		read++;
		write++;
	}

	*write = 0;
}

LJ_bool LJ_strIsHex( const LJ_char* const string, LJ_uint* const val )
{
	LJ_int length;
	LJ_uint value = 0;
	LJ_int index = 2;

	*val = 0;

	// must start 0x
	if ( !LJ_strIsNSameIgnoreCase( string, "0x", 2 ) )
	{
		return LJ_FALSE;
	}

	// must be 32-bit or less
	length = LJ_strGetLength( string );
	if ( length > 10 )
	{
		return LJ_FALSE;
	}

	// start at position two and work out value

	while ( index < length )
	{
		LJ_char c = LJ_strToUpperChar( string[index] );
		const LJ_int digit = LJ_strFindChar( "0123456789ABCDEF", c );

		value *= 16;
		if ( digit < 0 )
		{
			return LJ_FALSE;
		}

		value += digit;

		index++;
	}

	*val = value;
	return LJ_TRUE;
}

LJ_bool LJ_strIsInt( const LJ_char* const string, LJ_int* const val )
{
	LJ_int value = 0;
	LJ_int index = 0;
	LJ_bool isNegative = LJ_FALSE;

	if ( string[0] == '-' )
	{
		isNegative = LJ_TRUE;
		index++;
	}
	else if ( string[0] == '+' )
	{
		isNegative = LJ_FALSE;
		index++;
	}

	while ( string[index] != 0 )
	{
		LJ_char c = LJ_strToUpperChar( string[index] );
		LJ_int digit = LJ_strFindChar( "0123456789", c );

		value *= 10;
		if ( digit < 0 )
		{
			return LJ_FALSE;
		}

		value += digit;

		index++;
	}

	if ( isNegative )
	{
		value = -value;
	}

	*val = value;
	return LJ_TRUE;
}

LJ_int LJ_strFindTextIndex( const LJ_char* const string, const LJ_char* const find )
{
	const LJ_char* read = string;
	LJ_int length = LJ_strGetLength( find );

	while ( *read != 0 )
	{
		LJ_int i;
		for ( i = 0; i < length; i++ )
		{
			if ( read[i] != find[i] )
			{
				break;
			}
		}

		if ( i == length )
		{
			return read - string;
		}

		read++;
	}

	return -1;
}

LJ_bool LJ_strFindText( const LJ_char* const string, const LJ_char* const find )
{
	if ( LJ_strFindTextIndex( string, find ) == -1 )
	{
		return LJ_FALSE;
	}

	return LJ_TRUE;
}

LJ_int LJ_strFindTextIgnoreCaseIndex( const LJ_char* const string, const LJ_char* const find )
{
	const LJ_char* read = string;
	LJ_int length = LJ_strGetLength( find );

	while ( *read != 0 )
	{
		LJ_int i;
		for ( i = 0; i < length; i++ )
		{
			if ( LJ_strToUpperChar( read[i] ) != LJ_strToUpperChar( find[i] ) )
			{
				break;
			}
		}

		if ( i == length )
		{
			return read - string;
		}

		read++;
	}

	return -1;
}

LJ_bool LJ_strFindTextIgnoreCase( const LJ_char* const string, const LJ_char* const find )
{
	if ( LJ_strFindTextIgnoreCaseIndex( string, find ) == -1 )
	{
		return LJ_FALSE;
	}

	return LJ_TRUE;
}

LJ_int LJ_strCompare( const LJ_char* const string, const LJ_char* const compare )
{
	LJ_uint index = 0;

	do
	{
		if ( string[index] < compare[index] )
		{
			return -1;
		}

		if ( string[index] > compare[index] )
		{
			return 1;
		}

		index++;
	}
	while ( string[index - 1] );

	return 0;
}

LJ_uint LJ_strReadUTF8( const LJ_char** c )
{
	// TODO: huge amount of error checking that could be done to verify this is a valid UTF-8 sequence

	LJ_uint utf8 = *(*c);
	(*c)++;
	if ( ( utf8 & 0x80 ) == 0 )
	{
		// simple ASCII character
		return utf8;
	}

	if ( ( utf8 & 0xE0 ) == 0xC0 ) // binary 110xxxxx 10xxxxxx
	{
		// 2 bytes per character
		utf8 &= 0x1F;
		utf8 <<= 6;
		utf8 += ( *(*c) & 0x3F );
		(*c)++;
		return utf8;
	}

	if ( ( utf8 & 0xF0 ) == 0xE0 ) // binary 1110xxxx 10xxxxxx 10xxxxxx
	{
		// 3 bytes per character
		utf8 &= 0x0F;
		utf8 <<= 6;
		utf8 += ( *(*c) & 0x3F );
		(*c)++;
		utf8 <<= 6;
		utf8 += ( *(*c) & 0x3F );
		(*c)++;
		return utf8;
	}

	if ( ( utf8 & 0xF8 ) == 0xF0 ) // binary 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	{
		// 4 bytes per character
		utf8 &= 0x07;
		utf8 <<= 6;
		utf8 += ( *(*c) & 0x3F );
		(*c)++;
		utf8 <<= 6;
		utf8 += ( *(*c) & 0x3F );
		(*c)++;
		utf8 <<= 6;
		utf8 += ( *(*c) & 0x3F );
		(*c)++;
		return utf8;
	}

	if ( ( utf8 & 0xFC ) == 0xF8 ) // binary 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	{
		// 5 bytes per character
		utf8 &= 0x03;
		utf8 <<= 6;
		utf8 += ( *(*c) & 0x3F );
		(*c)++;
		utf8 <<= 6;
		utf8 += ( *(*c) & 0x3F );
		(*c)++;
		utf8 <<= 6;
		utf8 += ( *(*c) & 0x3F );
		(*c)++;
		utf8 <<= 6;
		utf8 += ( *(*c) & 0x3F );
		(*c)++;
		return utf8;
	}

	if ( ( utf8 & 0xFE ) == 0xFC ) // binary 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	{
		// 6 bytes per character
		utf8 &= 0x01;
		utf8 <<= 6;
		utf8 += ( *(*c) & 0x3F );
		(*c)++;
		utf8 <<= 6;
		utf8 += ( *(*c) & 0x3F );
		(*c)++;
		utf8 <<= 6;
		utf8 += ( *(*c) & 0x3F );
		(*c)++;
		utf8 <<= 6;
		utf8 += ( *(*c) & 0x3F );
		(*c)++;
		utf8 <<= 6;
		utf8 += ( *(*c) & 0x3F );
		(*c)++;
		return utf8;
	}

	LJ_assert( LJ_FALSE, ( "Unknown UTF8 sequence" ) );
	return 0;
}

LJ_uint LJ_strWriteUTF8( LJ_char** c, const LJ_uint unicode )
{
	if ( unicode < 0x80 )
	{
		// simple ASCII character
		*(*c) = (LJ_char)( unicode );
		(*c)++;
		return 1;
	}

	if ( unicode < 0x800 )
	{
		// 2 bytes
		*(*c) = (LJ_char)( 0xC0 + ( ( unicode >> 6 ) & 0x1F ) );
		(*c)++;
		*(*c) = (LJ_char)( 0x80 + ( unicode & 0x3F ) );
		(*c)++;
		return 2;
	}

	if ( unicode < 0x10000 )
	{
		// 3 bytes
		*(*c) = (LJ_char)( 0xE0 + ( ( unicode >> 12 ) & 0x0F ) );
		(*c)++;
		*(*c) = (LJ_char)( 0x80 + ( ( unicode >> 6 ) & 0x3F ) );
		(*c)++;
		*(*c) = (LJ_char)( 0x80 + ( unicode & 0x3F ) );
		(*c)++;
		return 3;
	}

	if ( unicode < 0x200000 )
	{
		// 4 bytes
		*(*c) = (LJ_char)( 0xF0 + ( ( unicode >> 18 ) & 0x07 ) );
		(*c)++;
		*(*c) = (LJ_char)( 0x80 + ( ( unicode >> 12 ) & 0x3F ) );
		(*c)++;
		*(*c) = (LJ_char)( 0x80 + ( ( unicode >> 6 ) & 0x3F ) );
		(*c)++;
		*(*c) = (LJ_char)( 0x80 + ( unicode & 0x3F ) );
		(*c)++;
		return 4;
	}

	if ( unicode < 0x4000000 )
	{
		// 5 bytes
		*(*c) = (LJ_char)( 0xF8 + ( ( unicode >> 24 ) & 0x03 ) );
		(*c)++;
		*(*c) = (LJ_char)( 0x80 + ( ( unicode >> 18 ) & 0x3F ) );
		(*c)++;
		*(*c) = (LJ_char)( 0x80 + ( ( unicode >> 12 ) & 0x3F ) );
		(*c)++;
		*(*c) = (LJ_char)( 0x80 + ( ( unicode >> 6 ) & 0x3F ) );
		(*c)++;
		*(*c) = (LJ_char)( 0x80 + ( unicode & 0x3F ) );
		(*c)++;
		return 5;
	}

	// 6 bytes
	*(*c) = (LJ_char)( 0xFC + ( ( unicode >> 30 ) & 0x01 ) );
	(*c)++;
	*(*c) = (LJ_char)( 0x80 + ( ( unicode >> 24 ) & 0x3F ) );
	(*c)++;
	*(*c) = (LJ_char)( 0x80 + ( ( unicode >> 18 ) & 0x3F ) );
	(*c)++;
	*(*c) = (LJ_char)( 0x80 + ( ( unicode >> 12 ) & 0x3F ) );
	(*c)++;
	*(*c) = (LJ_char)( 0x80 + ( ( unicode >> 6 ) & 0x3F ) );
	(*c)++;
	*(*c) = (LJ_char)( 0x80 + ( unicode & 0x3F ) );
	(*c)++;
	return 6;
}

//-------------------------------------------------------------------
// part string functions
//-------------------------------------------------------------------

// copy the rest string to buffer if the start matches compare, returns LJ_TRUE if it does match
LJ_bool LJ_strCopyAfterMatchIgnoreCase( const LJ_char* const string, const LJ_char* const compare, LJ_char* const buffer )
{
	LJ_int stringLength = LJ_strGetLength( compare );
	if ( LJ_strIsNSameIgnoreCase( string, compare, stringLength ) )
	{
		if (buffer!=LJ_NULL)
		{
			LJ_strCopy(buffer, &string[stringLength]);
		}
		return LJ_TRUE;
	}

	if ( buffer != LJ_NULL )
	{
		*buffer = 0;
	}
	return LJ_FALSE;
}

// is the first of string the same as the whole of compare
LJ_bool LJ_strIsFirstPartSame( const LJ_char* const string, const LJ_char* const compare )
{
	LJ_int index = 0;
	while ( compare[index] != 0 )
	{
		if ( string[index] != compare[index] )
		{
			return LJ_FALSE;
		}
		index++;
	}
	return LJ_TRUE;
}

LJ_bool LJ_strIsFirstPartSameIgnoreCase( const LJ_char* const string, const LJ_char* const compare )
{
	LJ_int index = 0;
	while ( compare[index] != 0 )
	{
		if ( LJ_strToUpperChar( string[index] ) != LJ_strToUpperChar( compare[index] ) )
		{
			return LJ_FALSE;
		}
		index++;
	}
	return LJ_TRUE;
}

// if the first part of string matches stringToSkip, skip past it
LJ_bool LJ_strSkipString( const LJ_char** string, const LJ_char* const stringToSkip )
{
	LJ_int index = 0;
	while ( stringToSkip[index] != 0 )
	{
		if ( (*string)[index] != stringToSkip[index] )
		{
			return LJ_FALSE;
		}
		index++;
	}
	(*string) += index;
	return LJ_TRUE;
}

LJ_bool LJ_strSkipStringIgnoreCase( const LJ_char** string, const LJ_char* const stringToSkip)
{
	LJ_int index = 0;
	while ( stringToSkip[index] != 0 )
	{
		if ( LJ_strToUpperChar( (*string)[index] ) != LJ_strToUpperChar( stringToSkip[index] ) )
		{
			return LJ_FALSE;
		}
		index++;
	}
	(*string) += index;
	return LJ_TRUE;
}

//-------------------------------------------------------------------
// character based functions
//-------------------------------------------------------------------

LJ_char LJ_strGetLastChar( const LJ_char* const string )
{
	if (string!=LJ_NULL)
	{
		LJ_int stringLength = LJ_strGetLength( string );
		if ( stringLength > 0 )
		{
			return string[stringLength-1];
		}
	}
	return(0);
}

void LJ_strSetLastChar( LJ_char* const string, const LJ_char c )
{
	if ( string != LJ_NULL )
	{
		LJ_int stringLength = LJ_strGetLength( string );
		if ( stringLength > 0 )
		{
			string[stringLength-1] = c;
		}
	}
}

void LJ_strAppendChar( LJ_char* const string, const LJ_char c )
{
	if ( string != LJ_NULL )
	{
		if ( c != 0 )
		{
			LJ_int stringLength = LJ_strGetLength( string );
			string[stringLength] = c;
			string[stringLength+1] = 0;
		}
	}
}


// is the last character of the string a slash
LJ_bool LJ_strIsLastCharSlash( const LJ_char* const string )
{
	const LJ_char c = LJ_strGetLastChar( string );
	return( LJ_strIsSlash( c ) );
}

// return '/' or '\\', depending on which is used by the string.
// if neither or both are used, returns defaultSlash
LJ_char LJ_strGetSlashToUse( const LJ_char* const string, const LJ_char defaultSlash )
{
	LJ_char slash = 0;
	const LJ_char* s = string;
	while ( *s != 0 )
	{
		const LJ_char c = *s;
		if ( LJ_strIsSlash( c ) )
		{
			if( ( slash != 0 ) && ( slash != c ) )
			{
				// both slashes being used!
				return defaultSlash;
			}
			slash = c;
		}
		s++;
	}
	if ( slash == 0 )
	{
		// neither slash
		return defaultSlash;
	}
	return slash;
}

// convert slashes
LJ_int LJ_strConvertSlashes( LJ_char* const string, const LJ_char slash )
{
	LJ_int slashCount = 0;

	const LJ_char* s = string;
	LJ_char* d = string;
	while ( *s != 0 )
	{
		const LJ_char c = *s;
		s++;

		if ( LJ_strIsSlash( c ) )
		{
			slashCount++;
			if ( slash != 0 )
			{
				*d = slash;
				d++;
			}
		}
		else
		{
			*d = c;
			d++;
		}
	}

	*d = 0;
	return slashCount;
}

LJ_char* LJ_strSkipSlashes( const LJ_char* const string )
{
	LJ_char* returnString = (LJ_char*)string;
	while ( LJ_strIsSlash( returnString[0] ) )
	{
		returnString++;
	}
	return returnString;
}

//remove slashes from the end of the string
void LJ_strRemoveSlashesFromEnd( LJ_char* const string )
{
	while ( LJ_strIsLastCharSlash( string ) )
	{
		LJ_strSetLastChar( string, 0 );
	}
}

//make sure there is a slash on the end of the string
void LJ_strMakeSureEndsWithSlash( LJ_char* const string, const LJ_char slash )
{
	if ( !LJ_strIsLastCharSlash( string ) )
	{
		LJ_strAppendChar( string, slash );
	}
}

LJ_char* LJ_strFindFirstSlash( const LJ_char* const string )
{
	LJ_char* returnString = (LJ_char*)string;
	while ( returnString[0] != 0 )
	{
		if ( LJ_strIsSlash( returnString[0] ) )
		{
			return returnString;
		}
		returnString++;
	}
	return LJ_NULL;
}

LJ_char* LJ_strFindLastSlash( const LJ_char* const string )
{
	LJ_char* lastSlash = LJ_NULL;
	LJ_char* s = (LJ_char*)string;
	while ( *s != 0 )
	{
		if ( LJ_strIsSlash( *s ) )
		{
			lastSlash = s;
		}
		s++;
	}
	return lastSlash;
}

// remove the rest of the string after the last slash, returns LJ_TRUE if anything removed
LJ_bool LJ_strRemoveAfterLastSlash( LJ_char* const string )
{
	LJ_char* const lastSlash = LJ_strFindLastSlash( string );
	if ( lastSlash == LJ_NULL )
	{
		return LJ_FALSE;
	}
	lastSlash[1] = 0;
	return LJ_TRUE;
}

// does the string contain a slash
LJ_bool LJ_strHasSlash(const LJ_char* const string)
{
	return ( LJ_strFindFirstSlash( string ) != LJ_NULL );
}

// remove the whitespace from the start of the string and return the ptr 
LJ_char* LJ_strEatWhiteSpace( const LJ_char* const string )
{
	LJ_char* returnString = (LJ_char*)string;
	while ( LJ_strIsWhiteSpace( returnString[0] ) )
	{
		returnString++;
	}
	return returnString;
}


