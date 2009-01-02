//*******************************************************************
//
// File: CoreStr.cpp                         
//
// Created: 10 Nov 2003
//
// Description: Provides a suite of replacement functions for stdlib string
//              functions.
//
//*******************************************************************

// system libraries
//#include <stdio.h>
#include <stdlib.h>
//#include <stdarg.h>

// main header
#ifndef _CORESTR_H_
#include "CoreStr.h"
#endif // _CORESTR_H_

// other libraries

// internal dependencies
#ifndef _COREVECTOR_H_
#include "CoreMaths/CoreVector.h"
#endif // _COREVECTOR_H_

#ifndef _COREHASH_H_
#include "CoreHash/CoreHash.h"
#endif // _COREHASH_H_

#ifndef _COREERROR_H_
#include "CoreError.h"
#endif // _COREERROR_H_

//*******************************************************************
// Constants
//*******************************************************************

// Set of constants related to CoreF32 to string conversion in AppendFloat
#define MANTISSA_SHIFT		23
#define FIXED_POINT_SHIFT	31

const CoreU64 DECIMAL_POINT	= (1UL << FIXED_POINT_SHIFT);
const CoreU64 DIV_DECIMAL = (DECIMAL_POINT * 10000000UL);
const CoreU64 MAX_DECIMAL = (DIV_DECIMAL * 10UL);
const CoreU64 MIN_DECIMAL = (DECIMAL_POINT / 10000000UL);
#define START_DIGIT			+8
#define MAX_DIGIT			-7
#define END_DIGIT			-3


//*******************************************************************
// Local Classes
//*******************************************************************

//-------------------------------------------------------------------
// Struct:      FormatFlags
//
// Description: Flags used by replacement printf functions to store format
//              specifiers for each field.
//-------------------------------------------------------------------
struct FormatFlags
{
	FormatFlags();

	void Reset();

	CoreS32 width;
	CoreS32 precision;
	CoreB8 longSize;
	CoreB8 leftAlign;
	CoreB8 signPrefix;
	CoreB8 zeroPrefix;
};

//*******************************************************************
// Function Definitions
//*******************************************************************

////////////////////////
// FormatFlags

//-------------------------------------------------------------------
// Function:    FormatFlags::FormatFlags
//
// Description: Constructor. Clears flags.
//-------------------------------------------------------------------
FormatFlags::FormatFlags()
{
	Reset();
}


//-------------------------------------------------------------------
// Function:    FormatFlags::Reset
//
// Description: Reset flags.
//-------------------------------------------------------------------
void FormatFlags::Reset()
{
	width = -1;
	precision = -1;
	leftAlign = CORE_FALSE;
	signPrefix = CORE_FALSE;
	zeroPrefix = CORE_FALSE;
	longSize = CORE_FALSE;
}


////////////////////////
// Internal functions

//-------------------------------------------------------------------
// Function:    AppendChars
//
// Description: Append character to string (multiple times if required).
//
// Parameters:  o_to            - The string being appended to.
//              i_currentLength - The current length of the string in o_to.
//              i_maxLength     - The maximum length of the string o_to.
//              i_number        - The number of times to append the character
//                                i_char.
//              i_char          - The character to append to the string.
//
// Returns:     The new length of the string after the characters are appended.
//-------------------------------------------------------------------
static CoreU32 AppendChars(CoreC8* const o_to, const CoreU32 i_currentLength, const CoreU32 i_maxLength, const CoreS32 i_number, const CoreC8 i_char)
{
	const CoreU32 maxLength = i_maxLength - 1;

	CoreU32 index = i_currentLength;
	for (CoreS32 count = 0; count < i_number; count++)
	{
		if (index == maxLength)
		{
			// string too long to fit; terminate here
			if ( o_to != CORE_NULL )
			{
				o_to[maxLength] = 0;
			}
			return maxLength;
		}

		if ( o_to != CORE_NULL )
		{
			o_to[index] = i_char;
		}
		index++;
	}

	return index;
}


//-------------------------------------------------------------------
// Function:    AppendString
//
// Description: Append string to another string.
//
// Parameters:  o_to            - The string to append to.
//              i_currentLength - The current length of the string o_to.
//              i_maxLength     - The maximum length of the string o_to.
//              i_append        - The string to append.
//              i_length        - The length of the string being appended.
//              i_flags         - The format specifier flags.
//
// Returns:     The length of the resulting string.
//-------------------------------------------------------------------
static CoreU32 AppendString(CoreC8* const o_to, const CoreU32 i_currentLength, const CoreU32 i_maxLength, const CoreC8* const i_append, const CoreU32 i_length, const FormatFlags &i_flags)
{
	CoreU32 index = i_currentLength;

	CoreU32 count = 0;

	// deal with width specification
	CoreS32 width = i_flags.width - i_length;
	if (width > 0)
	{
		if (i_flags.zeroPrefix == CORE_TRUE)
		{
			if (i_append[0] == '-' || i_append[0] == '+')
			{
				// need to move sign to start of prepended zeros
				index = AppendChars(o_to, index, i_maxLength, 1, i_append[0]);
				count++;
			}

			index = AppendChars(o_to, index, i_maxLength, width, '0');
		}
		else if (i_flags.leftAlign == CORE_FALSE)
		{
			index = AppendChars(o_to, index, i_maxLength, width, ' ');
		}
	}

	// put output buffer onto end of string
	const CoreU32 maxLength = i_maxLength - 1;

	for ( ; count < i_length; count++)
	{
		if (index == maxLength)
		{
			// string too long to fit; terminate here
			o_to[maxLength] = 0;
			return maxLength;
		}

		if ( o_to != CORE_NULL )
		{
			o_to[index] = i_append[count];
		}
		index++;
	}

	// pad with spaces if required
	if (width > 0 && i_flags.leftAlign == CORE_TRUE)
	{
		index = AppendChars(o_to, index, i_maxLength, width, ' ');
	}

	return index;
}


//-------------------------------------------------------------------
// Function:    AppendNumber
//
// Description: Append a number to the string.
//
// Parameters:  o_to            - The string to append to
//              i_currentLength - The current length of the string o_to.
//              i_maxLength     - The maximum length of the string o_to.
//              i_value         - The integer number to append.
//              i_flags         - The format specifier flags.
//
// Returns:     The length of the resulting string.
//-------------------------------------------------------------------
static CoreU32 AppendNumber(CoreC8* const o_to, const CoreU32 i_currentLength, const CoreU32 i_maxLength, const CoreS64 i_value, const FormatFlags &i_flags)
{
	// temporary output buffer
	CoreC8 output[16];
	CoreU32 index = 0;

	CoreU64 mantissa;

	if (i_value < 0)
	{
		// -ve number; make +ve and output sign
		mantissa = -i_value;
		output[index++] = '-';
	}
	else
	{
		// +ve number
		mantissa = i_value;

		if (i_flags.signPrefix == CORE_TRUE)
		{
			// output + sign if required
			output[index++] = '+';
		}
	}

	if (mantissa == 0)
	{
		// zero is special case
		output[index++] = '0';
	}
	else
	{
		// set divider to maximum digit possible
		// CoreU32 divider = 1000000000;
		CoreU64 divider = 1000000000000000000LL;

		// find first digit
		while (divider > mantissa)
		{
			divider /= 10;
		}

		// output digits
		while (divider > 0)
		{
			CoreU64 number = mantissa / divider;

			output[index++] = '0' + (CoreC8)number;

			mantissa -= (number * divider);
			divider /= 10;
		}
	}

	// put this buffer onto end of string
	return AppendString(o_to, i_currentLength, i_maxLength, output, index, i_flags);
}


//-------------------------------------------------------------------
// Function:    AppendHex
//
// Description: Append a number in hexadecimal format.
//
// Parameters:  o_to            - The string to append to.
//              i_currentLength - The current length of the string o_to.
//              i_maxLength     - The maximum length of the string o_to.
//              i_value         - The number to append.
//              i_flags         - The format specifier flags.
//
// Returns:     The length of the resulting string.
//-------------------------------------------------------------------
static CoreU32 AppendHex(CoreC8* const o_to, const CoreU32 i_currentLength, const CoreU32 i_maxLength, const CoreU64 i_value, const FormatFlags &i_flags)
{
	// temporary output buffer
	CoreC8 output[16];
	CoreU32 index = 0;

	// default is 8 digit -> equivalent of %08X for 32-bit int's
	// default is 16 digit -> equivalent of %016X for 64-bit int's
	const CoreS32 maxDigit = (i_flags.longSize == CORE_FALSE) ? 7 : 15;
	CoreB8 doOutput = (i_flags.width < 0)? CORE_TRUE : CORE_FALSE;
	for (CoreS32 digit = maxDigit; digit >= 0; digit--)
	{
		CoreU64 number = (i_value >> (digit * 4)) & 0xF;

		// look for the first non-zero character to start doing output (unless doing default output)
		if (number > 0 || doOutput == CORE_TRUE)
		{
			output[index++] = (number < 10)? '0' + (CoreC8)number : 'A' + (CoreC8)(number - 10);
			doOutput = CORE_TRUE;
		}
	}

	if (doOutput == CORE_FALSE)
	{
		// if no output, then must be an unpadded zero
		output[index++] = '0';
	}

	// put this buffer onto end of string
	return AppendString(o_to, i_currentLength, i_maxLength, output, index, i_flags);
}


//-------------------------------------------------------------------
// Function:    AppendFloat
//
// Description: Append a floating point value to a string.
//
// Parameters:  o_to            - The string to append to.
//              i_currentLength - The current length of the string o_to.
//              i_maxLength     - The maximum length of the string o_to.
//              i_value         - The floating point number to append.
//              i_flags         - The format specifier flags.
//
// Returns:     The resulting string length.
//-------------------------------------------------------------------
static CoreU32 AppendFloat(CoreC8* const o_to, const CoreU32 i_currentLength, const CoreU32 i_maxLength, const CoreF32 i_value, const FormatFlags &i_flags)
{
	// get CoreF32 as its binary representation
	const CoreU32 binary = *(CoreU32*)&i_value;

	// extract component parts
	const CoreU32 sign = (binary & 0x80000000);
	CoreS32 exponent = ((binary - sign) >> 23) - 127;
	CoreU64 mantissa = (binary & 0x007FFFFF) + 0x00800000;

	// turn into 33.31 fixed point number range which covers most numbers we'll want to deal with, right?
	exponent += FIXED_POINT_SHIFT - MANTISSA_SHIFT;

	// shift the mantissa into the required fixed point format
	if (exponent < 0)
	{
		mantissa >>= -exponent;
	}
	else if (exponent > 0)
	{
		mantissa <<= exponent;
	}

	// now output each number as we find it
	CoreS32 endDigit = (i_flags.precision < 0)? END_DIGIT : -i_flags.precision;
	if (endDigit < MAX_DIGIT)
	{
		endDigit = MAX_DIGIT;
	}

	// add a little bit so get correct rounding on smallest decimal place
	CoreU64 decimal = (MIN_DECIMAL / 2);
	for (CoreS32 digitIndex = MAX_DIGIT; digitIndex < endDigit; digitIndex++)
	{
		decimal *= 10;
	}
	mantissa += decimal;

	// ready to start outputting string
	CoreC8 output[32];
	CoreU32 index = 0;

	if (sign != 0)
	{
		// always output sign if -ve
		output[index++] = '-';
	}
	else if (i_flags.signPrefix == CORE_TRUE)
	{
		// only do +ve if required
		output[index++] = '+';
	}

	// check the exponent and mantissa are still in range
	if (exponent >= 64 - MANTISSA_SHIFT || mantissa >= MAX_DECIMAL)
	{
		// overflow
		CoreStr::Copy(&output[index], "HUGE");
		return AppendString(o_to, i_currentLength, i_maxLength, output, index + 4, i_flags);
	}
	else if (exponent < -MANTISSA_SHIFT || mantissa < MIN_DECIMAL)
	{
		// underflow
		const CoreU32 length = 2 - endDigit;
		CoreStr::NCopy(&output[index], "0.0000000", length + 1);
		return AppendString(o_to, i_currentLength, i_maxLength, output, index + length, i_flags);
	}

	CoreS32 digit = START_DIGIT;

	// don't output anything till we find the first number, or decimal point
	while (mantissa < DIV_DECIMAL)
	{
		mantissa *= 10;

		digit--;
		if (digit == 1)
		{
			// quit out if reach decimal point
			break;
		}
	}

	while (digit > endDigit)
	{
		CoreU32 number = mantissa / DIV_DECIMAL;

		// output number
		output[index++] = '0' + (CoreC8)number;

		// NOTE: method keeps multiplying up mantissa to get next digit
		mantissa -= number * DIV_DECIMAL;
		mantissa *= 10;

		if (digit == 1)
		{
			// output decimal point when appropriate
			output[index++] = '.';
		}

		digit--;
	}

	// put this buffer onto end of string
	return AppendString(o_to, i_currentLength, i_maxLength, output, index, i_flags);
}


//-------------------------------------------------------------------
// Function:    AppendBinary
//
// Description: Append a binary number to a string.
//
// Parameters:  o_to            - The string to append to.
//              i_currentLength - The current length of the string o_to.
//              i_maxLength     - The maximum length of the string o_to.
//              i_value         - The number to append.
//              i_flags         - The format specifier flags.
//
// Returns:     The length of the resulting string.
//-------------------------------------------------------------------
static CoreU32 AppendBinary(CoreC8* const o_to, const CoreU32 i_currentLength, const CoreU32 i_maxLength, const CoreU32 i_value, const FormatFlags &i_flags)
{
	// temporary output buffer
	CoreC8 output[33];
	CoreU32 index = 0;

	// default is 32 digit -> equivalent of %032b
	CoreB8 doOutput = (i_flags.width < 0)? CORE_TRUE : CORE_FALSE;
	for (CoreS32 digit = 31; digit >= 0; digit--)
	{
		CoreU32 bit = (i_value >> digit) & 0x1;

		// look for the first non-zero character to start doing output (unless doing default output)
		if (bit != 0 || doOutput == CORE_TRUE)
		{
			output[index++] = '0' + bit;
			doOutput = CORE_TRUE;
		}
	}

	// put this buffer onto end of string
	return AppendString(o_to, i_currentLength, i_maxLength, output, index, i_flags);
}


//-------------------------------------------------------------------
// Function:    AppendVector
//
// Description: Append a 4 component vector value to a string.
//
// Parameters:  o_to            - The string to append to.
//              i_currentLength - The current length of the string o_to.
//              i_maxLength     - The maximum length of the string o_to.
//              i_value         - The vector to append.
//              i_flags         - The format specifier flags (which specify
//                                the format for each component, rather
//                                than the vector as a whole).
//
// Returns:     The length of the resulting string.
//-------------------------------------------------------------------
static CoreU32 AppendVector(CoreC8* const o_to, const CoreU32 i_currentLength, const CoreU32 i_maxLength, const CoreVector* const i_value, const FormatFlags &i_flags)
{
	CoreU32 length = i_currentLength;

	// output each element separated by commas
	length = AppendFloat(o_to, length, i_maxLength, i_value->GetX(), i_flags);
	length = AppendChars(o_to, length, i_maxLength, 1, ',');
	length = AppendChars(o_to, length, i_maxLength, 1, ' ');

	length = AppendFloat(o_to, length, i_maxLength, i_value->GetY(), i_flags);
	length = AppendChars(o_to, length, i_maxLength, 1, ',');
	length = AppendChars(o_to, length, i_maxLength, 1, ' ');

	length = AppendFloat(o_to, length, i_maxLength, i_value->GetZ(), i_flags);
	length = AppendChars(o_to, length, i_maxLength, 1, ',');
	length = AppendChars(o_to, length, i_maxLength, 1, ' ');

	length = AppendFloat(o_to, length, i_maxLength, i_value->GetW(), i_flags);

	return length;
}


//-------------------------------------------------------------------
// Function:    AppendHash
//
// Description: Append a hash value to a string.
//
// Parameters:  o_to            - The string to append to.
//              i_currentLength - The current length of the string o_to.
//              i_maxLength     - The maximum length of the string o_to.
//              i_value         - The hash value to append.
//              i_flags         - The format specifier flags.
//
// Returns:     The length of the resulting string.
//-------------------------------------------------------------------
static CoreU32 AppendHash(CoreC8* const o_to, const CoreU32 i_currentLength, const CoreU32 i_maxLength, const CoreHash i_value, const FormatFlags &i_flags)
{
	CoreU32 length = i_currentLength;

	const CoreC8* const hashString = i_value.GetString();
	if (hashString != CORE_NULL)
	{
		// write out string first if it exists
		length = AppendString(o_to, length, i_maxLength, hashString, CoreStr::GetLength(hashString), i_flags);
	}
	else
	{
		length = AppendString(o_to, length, i_maxLength, "(unknown)", 9, i_flags);
	}

	length = AppendChars(o_to, length, i_maxLength, 1, ':');
	length = AppendHex(o_to, length, i_maxLength, i_value.GetHash(), i_flags);

	return length;
}


//////////////////////
// CoreStr

//-------------------------------------------------------------------
// Function:    CoreStr::VSPrint
//
// Description: Replacement for vsprintf i.e. write to a string using a
//              formatted variable argument list.
//				If o_to is CORE_NULL then doesn't output anything, returns the length of string
//				that would be required
//
// Parameters:  o_to            - The string to write to.
//              i_maxLength     - The maximum length of the string (including
//                                null terminator).
//              i_format        - The formatting string.
//              i_list          - The variable argument list.
//
// Returns:     The length of the resulting string.
//-------------------------------------------------------------------
CoreS32 CoreStr::VSPrint(CoreC8* const o_to, const CoreU32 i_maxLength, const CoreC8* const i_format, CoreVAList &i_list)
{
	if (i_maxLength==0)
	{
		return(0);
	}
	// allow space for terminator
	const CoreU32 maxLength = i_maxLength - 1;

	// parsing mode
	enum FCMode
	{
		FCM_NORMAL,
		FCM_FLAG,
		FCM_WIDTH,
		FCM_PRECISION,
		FCM_SIZE,
		FCM_FORMAT
	}
	mode = FCM_NORMAL;

	// get some flags regarding the formatting of numbers
	FormatFlags flags;

	// parse through format string until reach terminator, or run out of buffer space
	CoreU32 length = 0;
	for (CoreU32 index = 0; length < maxLength; index++)
	{
		const CoreC8 chr = i_format[index];
		if (chr == 0)
		{
			break;
		}

		if (mode == FCM_NORMAL)
		{
			if (chr == '%')
			{
				//percent symbol is start of format specification
				mode = FCM_FLAG;
				continue;
			}
			// anything else just gets copied normally
			if ( o_to != CORE_NULL )
			{
				o_to[length] = chr;
			}
			length++;
			continue;
		}

		if (mode == FCM_FLAG)
		{
			switch (chr)
			{
				case '-':
				{
					flags.leftAlign = CORE_TRUE;
					continue;
				}

				case '+':
				{
					flags.signPrefix = CORE_TRUE;
					continue;
				}

				case '0':
				{
					// NOTE: '-' overrides '0' specification
					if (flags.leftAlign == CORE_FALSE)
					{
						flags.zeroPrefix = CORE_TRUE;
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

		if (mode == FCM_WIDTH)
		{
			// read width specification
			if (chr >= '0' && chr <= '9')
			{
				if (flags.width < 0)
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

			if (chr == '.')
			{
				// dot marks the start of a precision field
				mode = FCM_PRECISION;
				continue;
			}

			mode = FCM_SIZE;
		}

		if (mode == FCM_PRECISION)
		{
			// read the precision specification
			if (chr >= '0' && chr <= '9')
			{
				if (flags.precision < 0)
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

			if (chr == '*')
			{
				// get precision from parameter
				flags.precision = CORE_VA_ARG(i_list, CoreS32);
				mode = FCM_SIZE;
				continue;
			}

			mode = FCM_SIZE;
		}

		if (mode == FCM_SIZE)
		{
			flags.longSize = CORE_FALSE;
			// size specifier - only 'l' is supported at the moment
			switch (chr)
			{
				case 'l':
				{
					// means long input e.g. int64 or wide character or wide string
					// only int64 is supported at the moment
					flags.longSize = CORE_TRUE;
					mode = FCM_FORMAT;
					continue;
					break;
				}
			}
			mode = FCM_FORMAT;
		}
		
		if (mode == FCM_FORMAT)
		{
			// interpret format character
			switch (chr)
			{
				case 'c':
				{
					// character
					const CoreC8 character = (CoreC8)CORE_VA_ARG(i_list, CoreS32);
					if ( o_to != CORE_NULL )
					{
						o_to[length] = character;
					}
					length++;
					break;
				}

				case 's':
				{
					// string
					const CoreC8* subString = CORE_VA_ARG(i_list, CoreC8*);
					if (subString == CORE_NULL)
					{
						// deal with CORE_NULL pointer
						subString = "(null)";
					}
						
					CoreS32 strLen = CoreStr::GetLength(subString);
					if (flags.precision > 0 && strLen > flags.precision)
					{
						// precision specifier can truncate strings
						strLen = flags.precision;
					}

					length = AppendString(o_to, length, i_maxLength, subString, strLen, flags);
					break;
				}

				case 'f':
				case 'g':
				case 'G':
				case 'e':
				case 'E':
				{
					const CoreF32 value			= (CoreF32)CORE_VA_ARG(i_list, CoreF64);
					length = AppendFloat(o_to, length, i_maxLength, value, flags);
					break;
				}

				case 'd':
				case 'i':
				case 'u':
					{
						// integer
						//const CoreS32 value = CORE_VA_ARG(i_list, CoreS32);
						const CoreS64 value = (flags.longSize == CORE_FALSE) ? CORE_VA_ARG(i_list, CoreS32) : CORE_VA_ARG(i_list, CoreS64);
						length = AppendNumber(o_to, length, i_maxLength, value, flags);
						break;
					}

				case 'p':
				case 'x':
				case 'X':
					{
						// hex integer
						//const CoreU32 value = CORE_VA_ARG(i_list, CoreU32);
						const CoreU64 value = (flags.longSize == CORE_FALSE) ? CORE_VA_ARG(i_list, CoreU32) : CORE_VA_ARG(i_list, CoreU64);
						length = AppendHex(o_to, length, i_maxLength, value, flags);
						break;
					}

				case 'v':
					{
						// vector
						const CoreVector* const value = CORE_VA_ARG(i_list, CoreVector*);
						length = AppendVector(o_to, length, i_maxLength, value, flags);
						break;
					}

				case 'b':
					{
						// binary
						const CoreU32 value = CORE_VA_ARG(i_list, CoreU32);
						length = AppendBinary(o_to, length, i_maxLength, value, flags);
						break;
					}

				case 'h':
					{
						// hash
						const CoreHash value = CORE_VA_ARG(i_list, CoreU32);
						length = AppendHash(o_to, length, i_maxLength, value, flags);
						break;
					}

				default:
					{
						// unknown character which just gets output
						if ( o_to != CORE_NULL )
						{
							o_to[length] = chr;
						}
						length++;
					}
			}

			// go back to normal string parsing
			mode = FCM_NORMAL;
			flags.Reset();
		}
	}

	// terminate the string
	if( ( o_to != CORE_NULL ) && (i_maxLength > 0) )
	{
		o_to[length] = 0;
	}
	return(length);
}


//-------------------------------------------------------------------
// Function:    CoreStr::SPrint
//
// Description: Replacement for sprintf i.e. write a formatted string.
//
// Parameters:  o_to            - The string to write to.
//              i_maxLength     - The maximum length of the string (including
//                                null terminator).
//              i_format        - The formatting string.
//              ...				- Variable argument list.
//
// Returns:     The length of the resulting string.
//-------------------------------------------------------------------
CoreS32 CoreStr::SPrint(CoreC8* const o_to, const CoreU32 i_maxLength, const CoreC8* const i_format, ...)
{
	CoreVAList args;
	CORE_VA_START(args, i_format);
	return VSPrint(o_to, i_maxLength, i_format, args);
}

//-------------------------------------------------------------------
// Function:    CoreStr::GetLength
//
// Description: Find the length of a string.
//
// Parameters:  i_string        - The string.
//
// Returns:     The length.
//-------------------------------------------------------------------
CoreS32 CoreStr::GetLength(const CoreC8* const i_string)
{
	const CoreC8* read = i_string;

	while (*read != 0)
	{
		read++;
	}

	return (CoreS32)(read - i_string);
}

//-------------------------------------------------------------------
// Function:    CoreStr::Copy
//
// Description: Copy a string.
//
// Parameters:  o_to            - The string to copy to.
//              i_from          - The string to copy from.
//-------------------------------------------------------------------
void CoreStr::Copy(CoreC8* const o_to, const CoreC8* const i_from)
{
	const CoreC8* read = i_from;
	CoreC8* write = o_to;

	while (*read != 0)
	{
		*write = *read;
		read++;
		write++;
	}

	*write = 0;
}


//-------------------------------------------------------------------
// Function:    CoreStr::NCopy
//
// Description: Copy a string. The maximum length (including null terminator)
//              of the destination string is specified, and cannot be exceeded.
//              The resulting string is always null terminated, even if
//              it is truncated due to lack of space.
//
// Parameters:  o_to            - The string to copy to.
//              i_from          - The string to copy from.
//              i_maxLength     - The maximum length (including null terminator)
//                                of o_to, hence up to one less than this
//                                number of characters can actually fit.
//-------------------------------------------------------------------
void CoreStr::NCopy(CoreC8* const o_to, const CoreC8* const i_from, const CoreU32 i_maxLength)
{
	CoreU32 index = 0;
	while (index < i_maxLength - 1)
	{
		o_to[index] = i_from[index];

		if (i_from[index] == 0)
		{
			// found terminator
			return;
		}

		index++;
	}

	// got to end of string - zero terminate
	o_to[index] = 0;
}


//-------------------------------------------------------------------
// Function:    CoreStr::IsSame
//
// Description: Check if the two strings are exactly the same.
//
// Parameters:  i_string        - The first string.
//              i_compare       - The string to compare with it.
//
// Returns:     CORE_TRUE if they are the same.
//-------------------------------------------------------------------
CoreB8 CoreStr::IsSame(const CoreC8* const i_string, const CoreC8* const i_compare)
{
	const CoreC8* readString = i_string;
	const CoreC8* readCompare = i_compare;

	while (*readString == *readCompare)
	{
		if (*readString == 0)
		{
			return CORE_TRUE;
		}

		readString++;
		readCompare++;
	}

	return CORE_FALSE;
}


//-------------------------------------------------------------------
// Function:    CoreStr::IsNSame
//
// Description: Compare two strings, but only up to a specified number
//              of characters.
//
// Parameters:  i_string        - The first string.
//              i_compare       - The string to compare with it.
//              i_maxLength     - The maximum number of characters to compare.
//
// Returns:     CORE_TRUE if the strings match.
//-------------------------------------------------------------------
CoreB8 CoreStr::IsNSame(const CoreC8* const i_string, const CoreC8* const i_compare, const CoreU32 i_maxLength)
{
	if (i_maxLength == 0)
	{
		return CORE_TRUE;
	}

	const CoreC8* readString = i_string;
	const CoreC8* readCompare = i_compare;
	CoreU32 length = 0;

	while (1)
	{
		if (length == i_maxLength)
		{
			return CORE_TRUE;
		}
		else if (*readString != *readCompare)
		{
			return CORE_FALSE;
		}

		if (*readString == 0)
		{
			return CORE_TRUE;
		}

		readString++;
		readCompare++;
		length++;
	}

	//  return CORE_FALSE;
}


//-------------------------------------------------------------------
// Function:    CoreStr::FindChar
//
// Description: Find the first occurrence of the specified character in
//              the string.
//
// Parameters:  i_string        - The string to search.
//              i_char          - The character to find.
//
// Returns:     The index of the character if found, or -1 if not in the
//              string at all.
//-------------------------------------------------------------------
CoreS32 CoreStr::FindChar(const CoreC8* const i_string, const CoreC8 i_char)
{
	const CoreC8* read = i_string;

	while (*read != 0)
	{
		if (*read == i_char)
		{
			return read - i_string;
		}

		read++;
	}

	return -1;
}


//-------------------------------------------------------------------
// Function:    CoreStr::ReverseFindChar
//
// Description: Find the last occurrence of the specified character in
//              the string.
//
// Parameters:  i_string        - The string to search.
//              i_char          - The character to find.
//
// Returns:     The index of the character, or -1 if it is not found.
//-------------------------------------------------------------------
CoreS32 CoreStr::ReverseFindChar(const CoreC8* const i_string, const CoreC8 i_char)
{
	// Catch the zero length string
	const CoreS32 length = GetLength(i_string);
	if (length == 0)
	{
		return -1;
	}

	const CoreC8* read = i_string + length - 1;

	while (*read != 0)
	{
		if (*read == i_char)
		{
			return read - i_string;
		}

		read--;
	}

	return -1;
}


//-------------------------------------------------------------------
// Function:    CoreStr::Cat
//
// Description: Append one string to another.
//
// Parameters:  o_to            - The string to append to.
//              i_from          - The string to append onto it.
//-------------------------------------------------------------------
void CoreStr::Cat(CoreC8* const o_to, const CoreC8* const i_from)
{
	CoreC8* write = o_to + GetLength(o_to);
	Copy(write, i_from);
}


//-------------------------------------------------------------------
// Function:    CoreStr::Cat
//
// Description: Append a character to the end of a string.
//
// Parameters:  o_to            - The string to append to.
//              i_char          - The character to append.
//-------------------------------------------------------------------
void CoreStr::Cat(CoreC8* const o_to, const CoreC8 i_char)
{
	CoreC8* write = o_to + GetLength(o_to);
	*write = i_char;
	*(write + 1) = 0;
}


//-------------------------------------------------------------------
// Function:    CoreStr::NCat
//
// Description: Append one string to another. The resulting string cannot
//              exceed the specified length (including null terminator).
//              The resulting string is always null terminated, even if
//              the appended string is truncated.
//
// Parameters:  o_to            - The string to append to.
//              i_from          - The string to append to it.
//              i_maxLength     - The maximum length (including null terminator)
//                                of the resulting string.
//-------------------------------------------------------------------
void CoreStr::NCat(CoreC8* const o_to, const CoreC8* const i_from, const CoreU32 i_maxLength)
{
	CoreU32 length = GetLength(o_to);

	NCopy(&o_to[length], i_from, i_maxLength - length);
}


//-------------------------------------------------------------------
// Function:    CoreStr::ToFloat
//
// Description: Convert the string to a floating point value.
//
// Parameters:  i_string        - The string.
//
// Returns:     The floating point value.
//-------------------------------------------------------------------
CoreF32 CoreStr::ToFloat(const CoreC8* const i_string)
{
#if PLATFORM_PSP == 1
	return (CoreF32)(atoi(i_string));
#else
	return (CoreF32)(atof(i_string));
#endif
}


//-------------------------------------------------------------------
// Function:    CoreStr::ToInt
//
// Description: Convert the string to an integer value.
//
// Parameters:  i_string        - The string.
//
// Returns:     The integer value.
//-------------------------------------------------------------------
CoreS32 CoreStr::ToInt(const CoreC8* const i_string)
{
	return (CoreS32)(atoi(i_string));
}


//-------------------------------------------------------------------
// Function:    CoreStr::IsSameIgnoreCase
//
// Description: Check if the two strings are the same, ignoring case for
//              alphanumeric characters.
//
// Parameters:  i_string        - The first string.
//              i_compare       - The string to compare to it.
//
// Returns:     CORE_TRUE if the strings are the same.
//-------------------------------------------------------------------
CoreB8 CoreStr::IsSameIgnoreCase(const CoreC8* const i_string, const CoreC8* const i_compare)
{
	const CoreC8* readString = i_string;
	const CoreC8* readCompare = i_compare;

	while (ToUpper(*readString) == ToUpper(*readCompare))
	{
		if (*readString == 0)
		{
			return CORE_TRUE;
		}

		readString++;
		readCompare++;
	}

	return CORE_FALSE;
}


//-------------------------------------------------------------------
// Function:    CoreStr::IsNSameIgnoreCase
//
// Description: Check if the two strings are the same, ignoring case for
//              alphanumeric characters. Only check up to a specified number
//              of characters.
//
// Parameters:  i_string        - The first string.
//              i_compare       - The string to compare to it.
//              i_maxLength     - The maximum number of characters to compare.
//
// Returns:     CORE_TRUE if the strings are the same.
//-------------------------------------------------------------------
CoreB8 CoreStr::IsNSameIgnoreCase(const CoreC8* const i_string, const CoreC8* const i_compare, const CoreU32 i_maxLength)
{
	const CoreC8* readString = i_string;
	const CoreC8* readCompare = i_compare;
	CoreU32 length = 0;

	CoreC8 char1;
	do
	{
		if (length == i_maxLength)
		{
			return CORE_TRUE;
		}

		char1 = ToUpper(*readString);
		const CoreC8 char2 = ToUpper(*readCompare);

		if (char1 != char2)
		{
			return CORE_FALSE;
		}

		readString++;
		readCompare++;
		length++;
	}
	while (char1 != 0);

	return CORE_TRUE;
}


//-------------------------------------------------------------------
// Function:    CoreStr::ToUpper
//
// Description: Get the upper case version of a character.
//
// Parameters:  i_char          - The character.
//
// Returns:     The upper case version.
//-------------------------------------------------------------------
CoreC8 CoreStr::ToUpper(const CoreC8 i_char)
{
	CoreC8 result = i_char;

	if ((i_char >= 'a') && (i_char <= 'z'))
	{
		result -= ('a' -'A');
	}

	return result;
}


//-------------------------------------------------------------------
// Function:    CoreStr::ToLower
//
// Description: Return the lower case version of a character.
//
// Parameters:  i_char          - The character.
//
// Returns:     The lower case version.
//-------------------------------------------------------------------
CoreC8 CoreStr::ToLower(const CoreC8 i_char)
{
	CoreC8 result = i_char;

	if ((i_char >= 'A') && (i_char <= 'Z'))
	{
		result += ('a' -'A');
	}

	return result;
}


//-------------------------------------------------------------------
// Function:    CoreStr::ToUpper
//
// Description: Create an upper case version of a string.
//
// Parameters:  o_to            - The resulting upper case string.
//              i_from          - The starting string.
//-------------------------------------------------------------------
void CoreStr::ToUpper(CoreC8* const o_to, const CoreC8* const i_from)
{
	const CoreC8* read = i_from;
	CoreC8* write = o_to;

	while (*read != 0)
	{
		*write = CoreStr::ToUpper(*read);
		read++;
		write++;
	}

	*write = 0;
}


//-------------------------------------------------------------------
// Function:    CoreStr::ToLower
//
// Description: Create a lower case version of a string.
//
// Parameters:  o_to            - The resulting lower case string.
//              i_from          - The starting string.
//-------------------------------------------------------------------
void CoreStr::ToLower(CoreC8* const o_to, const CoreC8* const i_from)
{
	const CoreC8* read = i_from;
	CoreC8* write = o_to;

	while (*read != 0)
	{
		*write = CoreStr::ToLower(*read);
		read++;
		write++;
	}

	*write = 0;
}


//-------------------------------------------------------------------
// Function:    CoreStr::IsHex
//
// Description: Check if a string contains a valid hexadecimal number
//				in the format 0x89ABCDEF.
//
// Parameters:  i_string        - The string.
//              o_value         - The hex value contained in the string.
//
// Returns:     CORE_TRUE if the string was a valid hex value.
//-------------------------------------------------------------------
CoreB8 CoreStr::IsHex(const CoreC8* const i_string, CoreU32 &o_value)
{
	o_value = 0;

	// must start 0x
	if (!IsNSameIgnoreCase(i_string, "0x", 2))
	{
		return CORE_FALSE;
	}

	// must be 32-bit or less
	CoreS32 length = CoreStr::GetLength(i_string);
	if (length > 10)
	{
		return CORE_FALSE;
	}

	// start at position two and work out value
	CoreU32 value = 0;
	CoreS32 index = 2;

	while (index < length)
	{
		value *= 16;
		CoreC8 i_char = ToUpper(i_string[index]);

		CoreS32 digit = FindChar("0123456789ABCDEF", i_char);
		if (digit < 0)
		{
			return CORE_FALSE;
		}

		value += digit;

		index++;
	}

	o_value = value;
	return CORE_TRUE;
}


//-------------------------------------------------------------------
// Function:    CoreStr::IsInt
//
// Description: Check if a string contains a valid integer value.
//
// Parameters:  i_string        - The string.
//              o_value         - The integer value.
//
// Returns:     CORE_TRUE if the string was a valid integer.
//-------------------------------------------------------------------
CoreB8 CoreStr::IsInt(const CoreC8* const i_string, CoreS32 &o_value)
{
	CoreS32 value = 0;
	CoreS32 index = 0;
	CoreB8 isNegative = CORE_FALSE;

	if (i_string[0] == '-')
	{
		isNegative = CORE_TRUE;
		index++;
	}

	CoreS32 length = CoreStr::GetLength(i_string);
	while (index < length)
	{
		value *= 10;
		CoreC8 i_char = ToUpper(i_string[index]);

		CoreS32 digit = FindChar("0123456789", i_char);
		if (digit < 0)
		{
			return CORE_FALSE;
		}

		value += digit;

		index++;
	}

	if (isNegative)
	{
		value = -value;
	}

	o_value = value;
	return CORE_TRUE;
}


//-------------------------------------------------------------------
// Function:    CoreStr::FindTextIndex
//
// Description: Search for first occurrence of a string within another
//              string.
//
// Parameters:  i_string        - The string to search.
//              i_find          - The string to find.
//
// Returns:     The index of the start of the found string, or -1 if it
//              was not found.
//-------------------------------------------------------------------
CoreS32 CoreStr::FindTextIndex(const CoreC8* const i_string, const CoreC8* i_find)
{
	const CoreC8* read = i_string;
	CoreS32 length = GetLength(i_find);

	while (*read != 0)
	{
		CoreS32 i;
		for (i = 0; i < length; i++)
		{
			if (read[i] != i_find[i])
			{
				break;
			}
		}

		if (i == length)
		{
			return read - i_string;
		}

		read++;
	}

	return -1;
}


//-------------------------------------------------------------------
// Function:    CoreStr::FindText
//
// Description: Check if a string of text exists within another string.
//
// Parameters:  i_string        - The string.
//              i_find          - The string to find.
//
// Returns:     CORE_TRUE if the string was found.
//-------------------------------------------------------------------
CoreB8 CoreStr::FindText(const CoreC8* const i_string, const CoreC8* i_find)
{
	if (FindTextIndex(i_string, i_find) == -1)
	{
		return CORE_FALSE;
	}

	return CORE_TRUE;
}


//-------------------------------------------------------------------
// Function:    CoreStr::FindTextIgnoreCaseIndex
//
// Description: Search for first occurrence of a string within another
//              string, ignoring case.
//
// Parameters:  i_string        - The string to search.
//              i_find          - The string to find.
//
// Returns:     The index of the start of the found string, or -1 if it
//              was not found.
//-------------------------------------------------------------------
CoreS32 CoreStr::FindTextIgnoreCaseIndex(const CoreC8* const i_string, const CoreC8* i_find)
{
	const CoreC8* read = i_string;
	CoreS32 length = GetLength(i_find);

	while (*read != 0)
	{
		CoreS32 i;
		for (i = 0; i < length; i++)
		{
			if (ToUpper(read[i]) != ToUpper(i_find[i]))
			{
				break;
			}
		}

		if (i == length)
		{
			return read - i_string;
		}

		read++;
	}

	return -1;
}


//-------------------------------------------------------------------
// Function:    CoreStr::FindTextIgnoreCase
//
// Description: Check if a string of text exists within another string,
//				ignoring case.
//
// Parameters:  i_string        - The string.
//              i_find          - The string to find.
//
// Returns:     CORE_TRUE if the string was found.
//-------------------------------------------------------------------
CoreB8 CoreStr::FindTextIgnoreCase(const CoreC8* const i_string, const CoreC8* i_find)
{
	if (FindTextIgnoreCaseIndex(i_string, i_find) == -1)
	{
		return CORE_FALSE;
	}

	return CORE_TRUE;
}


//-------------------------------------------------------------------
// Function:    CoreStr::Compare
//
// Description: Compare two strings.
//
// Parameters:  i_string        - The first string.
//              i_compare       - The string to compare with it.
//
// Returns:     0 if the same, -1 if "less than" and +1 if "greater than".
//-------------------------------------------------------------------
CoreS32 CoreStr::Compare(const CoreC8* const i_string, const CoreC8* const i_compare)
{
	CoreU32 index = 0;

	do
	{
		if (i_string[index] < i_compare[index])
		{
			return -1;
		}

		if (i_string[index] > i_compare[index])
		{
			return 1;
		}

		index++;
	}
	while (i_string[index - 1]);

	return 0;
}


//-------------------------------------------------------------------
// Function:    CoreStr::ReadUTF8
//
// Description: Read a character from a UTF-8 encoded character sequence.
//
// Parameters:  io_char          - Pointer to the sequence. This is incremented
//                                by the number of bytes read to extract
//                                a single character code.
//
// Returns:     The character code.
//-------------------------------------------------------------------
CoreU32 CoreStr::ReadUTF8(const CoreC8* &io_char)
{
	// TODO: huge amount of error checking that could be done to verify this is a valid UTF-8 sequence

	CoreU32 utf8 = *(io_char++);
	if ((utf8 & 0x80) == 0)
	{
		// simple ASCII character
		return utf8;
	}

	if ((utf8 & 0xE0) == 0xC0) // binary 110xxxxx 10xxxxxx
	{
		// 2 bytes per character
		utf8 &= 0x1F;
		utf8 <<= 6;
		utf8 += *(io_char++) & 0x3F;
		return utf8;
	}

	if ((utf8 & 0xF0) == 0xE0) // binary 1110xxxx 10xxxxxx 10xxxxxx
	{
		// 3 bytes per character
		utf8 &= 0x0F;
		utf8 <<= 6;
		utf8 += *(io_char++) & 0x3F;
		utf8 <<= 6;
		utf8 += *(io_char++) & 0x3F;
		return utf8;
	}

	if ((utf8 & 0xF8) == 0xF0) // binary 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	{
		// 4 bytes per character
		utf8 &= 0x07;
		utf8 <<= 6;
		utf8 += *(io_char++) & 0x3F;
		utf8 <<= 6;
		utf8 += *(io_char++) & 0x3F;
		utf8 <<= 6;
		utf8 += *(io_char++) & 0x3F;
		return utf8;
	}

	if ((utf8 & 0xFC) == 0xF8) // binary 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	{
		// 5 bytes per character
		utf8 &= 0x03;
		utf8 <<= 6;
		utf8 += *(io_char++) & 0x3F;
		utf8 <<= 6;
		utf8 += *(io_char++) & 0x3F;
		utf8 <<= 6;
		utf8 += *(io_char++) & 0x3F;
		utf8 <<= 6;
		utf8 += *(io_char++) & 0x3F;
		return utf8;
	}

	if ((utf8 & 0xFE) == 0xFC) // binary 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	{
		// 6 bytes per character
		utf8 &= 0x01;
		utf8 <<= 6;
		utf8 += *(io_char++) & 0x3F;
		utf8 <<= 6;
		utf8 += *(io_char++) & 0x3F;
		utf8 <<= 6;
		utf8 += *(io_char++) & 0x3F;
		utf8 <<= 6;
		utf8 += *(io_char++) & 0x3F;
		utf8 <<= 6;
		utf8 += *(io_char++) & 0x3F;
		return utf8;
	}

	CoreAssert(0); // not a valid UTF-8 escape sequence at all
	return 0;
}


//-------------------------------------------------------------------
// Function:    CoreStr::WriteUTF8
//
// Description: Converts unicode character into UTF-8 character sequence.
//
// Parameters:  io_char         - The character buffer to write to (pointer
//                                will be incremented).
//              i_unicode       - The unicode character to convert.
//
// Returns:     The number of UTF-8 characters written.
//-------------------------------------------------------------------
CoreU32 CoreStr::WriteUTF8(CoreC8* &io_char, const CoreU32 i_unicode)
{
	if (i_unicode < 0x80)
	{
		// simple ASCII character
		*(io_char++) = i_unicode;
		return 1;
	}

	if (i_unicode < 0x800)
	{
		// 2 bytes
		*(io_char++) = 0xC0 + ((i_unicode >> 6) & 0x1F);
		*(io_char++) = 0x80 + (i_unicode & 0x3F);
		return 2;
	}

	if (i_unicode < 0x10000)
	{
		// 3 bytes
		*(io_char++) = 0xE0 + ((i_unicode >> 12) & 0x0F);
		*(io_char++) = 0x80 + ((i_unicode >> 6) & 0x3F);
		*(io_char++) = 0x80 + (i_unicode & 0x3F);
		return 3;
	}

	if (i_unicode < 0x200000)
	{
		// 4 bytes
		*(io_char++) = 0xF0 + ((i_unicode >> 18) & 0x07);
		*(io_char++) = 0x80 + ((i_unicode >> 12) & 0x3F);
		*(io_char++) = 0x80 + ((i_unicode >> 6) & 0x3F);
		*(io_char++) = 0x80 + (i_unicode & 0x3F);
		return 4;
	}

	if (i_unicode < 0x4000000)
	{
		// 5 bytes
		*(io_char++) = 0xF8 + ((i_unicode >> 24) & 0x03);
		*(io_char++) = 0x80 + ((i_unicode >> 18) & 0x3F);
		*(io_char++) = 0x80 + ((i_unicode >> 12) & 0x3F);
		*(io_char++) = 0x80 + ((i_unicode >> 6) & 0x3F);
		*(io_char++) = 0x80 + (i_unicode & 0x3F);
		return 5;
	}

	// 6 bytes
	*(io_char++) = 0xFC + ((i_unicode >> 30) & 0x01);
	*(io_char++) = 0x80 + ((i_unicode >> 24) & 0x3F);
	*(io_char++) = 0x80 + ((i_unicode >> 18) & 0x3F);
	*(io_char++) = 0x80 + ((i_unicode >> 12) & 0x3F);
	*(io_char++) = 0x80 + ((i_unicode >> 6) & 0x3F);
	*(io_char++) = 0x80 + (i_unicode & 0x3F);
	return 6;
}

//-------------------------------------------------------------------
// part string functions
//-------------------------------------------------------------------
//copy the rest i_string to o_buffer if the start matches i_compare, returns CORE_TRUE if it does match
CoreBool CoreStr::CopyAfterMatchIgnoreCase(const CoreC8* const i_string, const CoreC8* const i_compare, CoreC8* const o_buffer)
{
	CoreS32 stringLength = GetLength(i_compare);
	if (IsNSameIgnoreCase(i_string, i_compare, stringLength))
	{
		if (o_buffer!=CORE_NULL)
		{
			Copy(o_buffer, &i_string[stringLength]);
		}
		return(CORE_TRUE);
	}

	if (o_buffer!=CORE_NULL)
	{
		o_buffer[0] = 0;
	}
	return(CORE_FALSE);
}

//is the first of i_string the same as the whole of i_compare
CoreBool CoreStr::IsFirstPartSame(const CoreC8* const i_string, const CoreC8* const i_compare)
{
	CoreS32 index = 0;
	while (i_compare[index] != 0)
	{
		if (i_string[index] != i_compare[index])
		{
			return(CORE_FALSE);
		}
		index++;
	}
	return(CORE_TRUE);
}

CoreBool CoreStr::IsFirstPartSameIgnoreCase(const CoreC8* const i_string, const CoreC8* const i_compare)
{
	CoreS32 index = 0;
	while (i_compare[index] != 0)
	{
		if (ToUpper(i_string[index]) != ToUpper(i_compare[index]))
		{
			return(CORE_FALSE);
		}
		index++;
	}
	return(CORE_TRUE);
}

//if the first part of i_string matches i_stringToSkip, skip past it
CoreBool CoreStr::SkipString(CoreC8*& i_string, const CoreC8* const i_stringToSkip)
{
	CoreS32 index = 0;
	while (i_stringToSkip[index] != 0)
	{
		if (i_string[index] != i_stringToSkip[index])
		{
			return(CORE_FALSE);
		}
		index++;
	}
	i_string += index;
	return(CORE_TRUE);
}
CoreBool CoreStr::SkipString(const CoreC8*& i_string, const CoreC8* const i_stringToSkip)
{
	CoreS32 index = 0;
	while (i_stringToSkip[index] != 0)
	{
		if (i_string[index] != i_stringToSkip[index])
		{
			return(CORE_FALSE);
		}
		index++;
	}
	i_string += index;
	return(CORE_TRUE);
}
CoreBool CoreStr::SkipStringIgnoreCase(CoreC8*& i_string, const CoreC8* const i_stringToSkip)
{
	CoreS32 index = 0;
	while (i_stringToSkip[index] != 0)
	{
		if (ToUpper(i_string[index]) != ToUpper(i_stringToSkip[index]))
		{
			return(CORE_FALSE);
		}
		index++;
	}
	i_string += index;
	return(CORE_TRUE);
}
CoreBool CoreStr::SkipStringIgnoreCase(const CoreC8*& i_string, const CoreC8* const i_stringToSkip)
{
	CoreS32 index = 0;
	while (i_stringToSkip[index] != 0)
	{
		if (ToUpper(i_string[index]) != ToUpper(i_stringToSkip[index]))
		{
			return(CORE_FALSE);
		}
		index++;
	}
	i_string += index;
	return(CORE_TRUE);
}

//-------------------------------------------------------------------
// character based functions
//-------------------------------------------------------------------

CoreC8 CoreStr::GetLastChar(const CoreC8* const i_string)
{
	if (i_string!=CORE_NULL)
	{
		CoreS32 stringLength = GetLength(i_string);
		if (stringLength>0)
		{
			return(i_string[stringLength-1]);
		}
	}
	return(0);
}
void CoreStr::SetLastChar(CoreC8* const io_string, CoreC8 i_char)
{
	if (io_string != CORE_NULL)
	{
		CoreS32 stringLength = GetLength(io_string);
		if (stringLength>0)
		{
			io_string[stringLength-1] = i_char;
		}
	}
}

void CoreStr::AppendChar(CoreC8* const io_string, CoreC8 i_char)
{
	if (io_string != CORE_NULL)
	{
		if (i_char != 0)
		{
			CoreS32 stringLength = GetLength(io_string);
			io_string[stringLength] = i_char;
			io_string[stringLength+1] = 0;
		}
	}
}


//is the last character of the string a slash
CoreBool CoreStr::IsLastCharSlash(const CoreC8* const i_string)
{
	CoreC8 c = GetLastChar(i_string);
	return(IsSlash(c));
}

//return '/' or '\\', depending on which is used by the string.
//if neither or both are used, returns i_defaultSlash
CoreC8 CoreStr::GetSlashToUse(const CoreC8* const i_string, const CoreC8 i_defaultSlash)
{
	CoreC8 slash = 0;
	const CoreC8* s = i_string;
	while (s[0] != 0)
	{
		CoreC8 c = s[0];
		if (IsSlash(c))
		{
			if( (slash != 0) && (slash != c) )
			{
				//both slashes being used!
				return(i_defaultSlash);
			}
			slash = c;
		}
		s++;
	}
	if (slash == 0)
	{
		//neither slash
		return(i_defaultSlash);
	}
	return(slash);
}

//convert slashes
CoreS32 CoreStr::ConvertSlashes(CoreC8* const io_string, const CoreC8 i_slash)
{
	CoreS32 slashCount = 0;

	const CoreC8* s = io_string;
	CoreC8* d = io_string;
	while (s[0] != 0)
	{
		CoreC8 c = s[0];
		s++;

		if (IsSlash(c))
		{
			slashCount++;
			if (i_slash != 0)
			{
				d[0] = i_slash;
				d++;
			}
		}
		else
		{
			d[0] = c;
			d++;
		}
	}

	d[0] = 0;
	return(slashCount);
}

CoreC8* CoreStr::SkipSlashes(CoreC8* const i_string)
{
	CoreC8* returnString = i_string;
	while (IsSlash(returnString[0]))
	{
		returnString++;
	}
	return(returnString);
}
const CoreC8* CoreStr::SkipSlashes(const CoreC8* const i_string)
{
	const CoreC8* returnString = i_string;
	while (IsSlash(returnString[0]))
	{
		returnString++;
	}
	return(returnString);
}

//remove slashes from the end of the string
void CoreStr::RemoveSlashesFromEnd(CoreC8* const io_string)
{
	while (IsLastCharSlash(io_string))
	{
		SetLastChar(io_string, 0);
	}
}

//make sure there is a slash on the end of the string
void CoreStr::MakeSureEndsWithSlash(CoreC8* const io_string, const CoreC8 i_slash)
{
	if (!IsLastCharSlash(io_string))
	{
		AppendChar(io_string, i_slash);
	}
}

CoreC8* CoreStr::FindFirstSlash(CoreC8* const i_string)
{
	CoreC8* returnString = i_string;
	while (returnString[0]!=0)
	{
		if (IsSlash(returnString[0]))
		{
			return(returnString);
		}
		returnString++;
	}
	return(CORE_NULL);
}
const CoreC8* CoreStr::FindFirstSlash(const CoreC8* const i_string)
{
	const CoreC8* returnString = i_string;
	while (returnString[0]!=0)
	{
		if (IsSlash(returnString[0]))
		{
			return(returnString);
		}
		returnString++;
	}
	return(CORE_NULL);
}

CoreC8* CoreStr::FindLastSlash(CoreC8* const i_string)
{
	CoreC8* lastSlash = CORE_NULL;
	CoreC8* s = i_string;
	while (s[0]!=0)
	{
		if (IsSlash(s[0]))
		{
			lastSlash = s;
		}
		s++;
	}
	return(lastSlash);
}
const CoreC8* CoreStr::FindLastSlash(const CoreC8* const i_string)
{
	const CoreC8* lastSlash = CORE_NULL;
	const CoreC8* s = i_string;
	while (s[0]!=0)
	{
		if (IsSlash(s[0]))
		{
			lastSlash = s;
		}
		s++;
	}
	return(lastSlash);
}

//remove the rest of the string after the last slash, returns CORE_TRUE if anything removed
CoreBool CoreStr::RemoveAfterLastSlash(CoreC8* const io_string)
{
	CoreC8* lastSlash = FindLastSlash(io_string);
	if (lastSlash == CORE_NULL)
	{
		return(CORE_FALSE);
	}
	lastSlash[1] = 0;
	return(CORE_TRUE);
}

//does the string contain a slash
CoreBool CoreStr::HasSlash(const CoreC8* const i_string)
{
	return(FindFirstSlash(i_string) != CORE_NULL);
}

//*******************************************************************
//
// File name:			$File: //depot/Source/CoreEngine/CoreLibCommon/CoreBase/CoreStr.cpp $
// Last Modified:		$Author: GavinR $ $DateTime: 2004/05/11 11:48:13 $
// Version No:			$Revision: #26 $
//
// (c) 2003 Core Design Ltd
//
// Warning: This computer program is protected by copyright law and 
// international treaties. Unauthorized reproduction or distribution
// of this program, or any portion of it, may result in severe civil and
// criminal penalties, and will be prosecuted to the maximum extent 
// possible under the law.
//
//*******************************************************************

