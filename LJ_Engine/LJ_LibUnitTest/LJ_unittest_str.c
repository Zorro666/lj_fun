#include "LJ.h"
#include "LJ_unittest.h"
#include "LJ_LibCommon/LJ_LibCommon.h"

#include <math.h>

#if LJ_USE_UNITTEST

#define LJ_UNITTEST_STR_MAX_LEN 1024
LJ_COMPILE_TIME_ASSERT_GLOBAL( LJ_UNITTEST_STR_MAX_LEN > 11 );

LJ_UNITTEST_FUNCTION_START( str, fundamental )
{
	LJ_char buffer1[LJ_UNITTEST_STR_MAX_LEN];
	LJ_char buffer2[LJ_UNITTEST_STR_MAX_LEN];

	LJ_COMPILE_TIME_ASSERT( LJ_UNITTEST_STR_MAX_LEN > 11 );

	// The LJ_mem tests have already been done so I can trust the LJ_mem functions
	LJ_memZero( buffer1, LJ_UNITTEST_STR_MAX_LEN );
	LJ_memZero( buffer2, LJ_UNITTEST_STR_MAX_LEN );
	LJ_memCopy( buffer1, "0123456789", 11 );

	LJ_UNITTEST_EQUALS( LJ_strGetLength( "abc" ) , 3 );

	LJ_UNITTEST_TRUE( ( LJ_strCompare( "abc", "abc") == 0 ) );

	LJ_strCopy( buffer2, buffer1 );
	LJ_UNITTEST_TRUE( ( LJ_strCompare( buffer1, buffer2 ) == 0 ) );

	LJ_strNCopy( buffer2, buffer1, 4 );
	buffer1[3] ='\0';
	buffer2[3] ='\0';
	LJ_UNITTEST_TRUE( ( LJ_strCompare( buffer1, buffer2 ) == 0 ) );

	LJ_strCopy( buffer1, "ABC" );
	LJ_strCopy( buffer2, "abc" );

	LJ_UNITTEST_TRUE( ( LJ_strCompare( buffer1, buffer2 ) < 0 ) );

	LJ_strCat( buffer1, "ABC" );
	LJ_UNITTEST_TRUE( ( LJ_strCompare( buffer1, "ABCABC" ) == 0 ) );

	LJ_strCatChar( buffer1, '*' );
	LJ_UNITTEST_TRUE( ( LJ_strCompare( buffer1, "ABCABC*" ) == 0 ) );

	LJ_strNCat( buffer1, "0123456789", 13 );
	LJ_UNITTEST_TRUE( ( LJ_strCompare( buffer1, "ABCABC*01234" ) == 0 ) );
}
LJ_UNITTEST_FUNCTION_END( str, fundamental )

LJ_UNITTEST_FUNCTION_START( str, compare )
{
	LJ_char buffer1[LJ_UNITTEST_STR_MAX_LEN];
	LJ_char buffer2[LJ_UNITTEST_STR_MAX_LEN];

	LJ_COMPILE_TIME_ASSERT( LJ_UNITTEST_STR_MAX_LEN > 11 );

	// The LJ_mem tests have already been done so I can trust the LJ_mem functions
	LJ_memZero( buffer1, LJ_UNITTEST_STR_MAX_LEN );
	LJ_memZero( buffer2, LJ_UNITTEST_STR_MAX_LEN );

	LJ_strCopy( buffer1, "0123456789" );
	LJ_strCopy( buffer2, "0123456789" );

	LJ_UNITTEST_TRUE( LJ_strIsSame( buffer1, buffer2 ) );
	buffer2[5] = 'J';
	LJ_UNITTEST_TRUE( LJ_strIsNSame( buffer1, buffer2, 5 ) );
	LJ_UNITTEST_TRUE( ( LJ_strIsSame( buffer1, buffer2 ) == LJ_FALSE ) );

	LJ_strCopy( buffer1, "ABCDEFGHIJ" );
	LJ_strCopy( buffer2, "abcdefghij" );
	LJ_UNITTEST_TRUE( LJ_strIsSameIgnoreCase( buffer1, buffer2 ) );
	buffer2[5] = '*';
	LJ_UNITTEST_TRUE( LJ_strIsNSameIgnoreCase( buffer1, buffer2, 5 ) );
}
LJ_UNITTEST_FUNCTION_END( str, compare )

LJ_UNITTEST_FUNCTION_START( str, find )
{
	LJ_char buffer1[LJ_UNITTEST_STR_MAX_LEN];
	LJ_char buffer2[LJ_UNITTEST_STR_MAX_LEN];

	LJ_COMPILE_TIME_ASSERT( LJ_UNITTEST_STR_MAX_LEN > 21 );

	// The LJ_mem tests have already been done so I can trust the LJ_mem functions
	LJ_memZero( buffer1, LJ_UNITTEST_STR_MAX_LEN );
	LJ_memZero( buffer2, LJ_UNITTEST_STR_MAX_LEN );

	LJ_strCopy( buffer1, "01234567890123456789" );
	LJ_strCopy( buffer2, "01234567890123456789" );

	LJ_UNITTEST_EQUALS( LJ_strFindChar( buffer1, '5' ), 5 );
	LJ_UNITTEST_EQUALS( LJ_strReverseFindChar( buffer1, '5' ), 15 );

	LJ_UNITTEST_EQUALS( LJ_strFindTextIndex( buffer1, "789" ), 7 );
	LJ_UNITTEST_TRUE( LJ_strFindText( buffer1, "789" ) );
	LJ_UNITTEST_TRUE( ( LJ_strFindText( buffer1, "jake" ) == LJ_FALSE ) );

	LJ_strCopy( buffer1, "ABCDEFGHIJKLMNOPQRST" );
	LJ_strCopy( buffer2, "abcdefghijklmnopqrst" );

	LJ_UNITTEST_EQUALS( LJ_strFindTextIgnoreCaseIndex( buffer1, "jkl" ), 9 );
	LJ_UNITTEST_TRUE( LJ_strFindTextIgnoreCase( buffer1, "jkl" ) );
	LJ_UNITTEST_TRUE( ( LJ_strFindTextIgnoreCase( buffer1, "789" ) == LJ_FALSE ) );
}
LJ_UNITTEST_FUNCTION_END( str, find )

LJ_UNITTEST_FUNCTION_START( str, convert )
{
	LJ_char buffer1[LJ_UNITTEST_STR_MAX_LEN];
	LJ_char buffer2[LJ_UNITTEST_STR_MAX_LEN];
	LJ_char buffer3[LJ_UNITTEST_STR_MAX_LEN];
	LJ_int intVal;
	LJ_uint hexVal;

	LJ_COMPILE_TIME_ASSERT( LJ_UNITTEST_STR_MAX_LEN > 21 );

	// The LJ_mem tests have already been done so I can trust the LJ_mem functions
	LJ_memZero( buffer1, LJ_UNITTEST_STR_MAX_LEN );
	LJ_memZero( buffer2, LJ_UNITTEST_STR_MAX_LEN );

	LJ_strCopy( buffer1, "abcdefghijklmnopqrst" );
	LJ_strCopy( buffer2, "ABCDEFGHIJKLMNOPQRST" );

	LJ_strToUpper( buffer3, buffer1 );
	LJ_UNITTEST_TRUE( LJ_strIsSame( buffer3, buffer2 ) );
	LJ_strToLower( buffer3, buffer2 );
	LJ_UNITTEST_TRUE( LJ_strIsSame( buffer3, buffer1 ) );

	LJ_UNITTEST_EQUALS( LJ_strToUpperChar( 'a' ), 'A' );
	LJ_UNITTEST_EQUALS( LJ_strToLowerChar( 'A' ), 'a' );

	LJ_UNITTEST_EQUALS( LJ_strToLowerChar( 'A' ), 'a' );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_strToFloat( "0.123456" ), 0.123456f, 0.0f );
	LJ_UNITTEST_EQUALS( LJ_strToInt( "123456" ), 123456 );
	LJ_UNITTEST_EQUALS( LJ_strToInt( "-123456" ), -123456 );
	LJ_UNITTEST_TRUE( LJ_strIsHex( "0xDEADBEAF", &hexVal ) );
	LJ_UNITTEST_EQUALS( hexVal, 0xDEADBEAF );
	LJ_UNITTEST_TRUE( LJ_strIsInt( "123456", &intVal ) );
	LJ_UNITTEST_EQUALS( intVal, 123456 );
	LJ_UNITTEST_TRUE( LJ_strIsInt( "-123456", &intVal ) );
	LJ_UNITTEST_EQUALS( intVal, -123456 );

	LJ_UNITTEST_EQUALS( LJ_strSPrint( buffer3, LJ_UNITTEST_STR_MAX_LEN, "%d %d", 123, 456 ), 7 );
	LJ_UNITTEST_TRUE( LJ_strIsSame( buffer3, "123 456" ) );
}
LJ_UNITTEST_FUNCTION_END( str, convert )

LJ_UNITTEST_FUNCTION_START( str, utf8 )
{
	LJ_char utf8str1[16];
	const LJ_char* utf8ptr1;

	LJ_char utf8str2[16];
	LJ_char* utf8ptr2;

	LJ_memZero( utf8str1, 16 );
	LJ_memZero( utf8str2, 16 );

	utf8str1[0] = 'A';
	utf8ptr1 = utf8str1;
	LJ_UNITTEST_EQUALS( LJ_strReadUTF8( &utf8ptr1 ), 'A' );

	utf8str1[0] = 0xC2;
	utf8str1[1] = 0xA2;
	utf8ptr1 = utf8str1;
	LJ_UNITTEST_EQUALS( LJ_strReadUTF8( &utf8ptr1 ), 0xA2 );

	utf8str1[0] = 0xE2;
	utf8str1[1] = 0x82;
	utf8str1[2] = 0xAC;
	utf8ptr1 = utf8str1;
	LJ_UNITTEST_EQUALS( LJ_strReadUTF8( &utf8ptr1 ), 0x20AC );

	utf8str1[0] = 0xF4;
	utf8str1[1] = 0x8A;
	utf8str1[2] = 0xAF;
	utf8str1[3] = 0x8D;
	utf8ptr1 = utf8str1;
	LJ_UNITTEST_EQUALS( LJ_strReadUTF8( &utf8ptr1 ), 0x10ABCD );

	utf8str1[0] = 'A';
	utf8str1[1] = '\0';
	utf8ptr2 = utf8str2;
	LJ_UNITTEST_EQUALS( LJ_strWriteUTF8( &utf8ptr2, 'A' ), 1 );
	utf8str1[1] = '\0';
	LJ_UNITTEST_TRUE( LJ_strIsSame( utf8str1, utf8str2 ) );

	utf8str1[0] = 0xC2;
	utf8str1[1] = 0xA2;
	utf8str1[2] = '\0';
	utf8ptr2 = utf8str2;
	LJ_UNITTEST_EQUALS( LJ_strWriteUTF8( &utf8ptr2, 0xA2 ), 2 );
	utf8str2[2] = '\0';
	LJ_UNITTEST_TRUE( LJ_strIsSame( utf8str1, utf8str2 ) );

	utf8str1[0] = 0xE2;
	utf8str1[1] = 0x82;
	utf8str1[2] = 0xAC;
	utf8str1[3] = '\0';
	utf8ptr2 = utf8str2;
	LJ_UNITTEST_EQUALS( LJ_strWriteUTF8( &utf8ptr2, 0x20AC ), 3 );
	utf8str2[3] = '\0';
	LJ_UNITTEST_TRUE( LJ_strIsSame( utf8str1, utf8str2 ) );

	utf8str1[0] = 0xF4;
	utf8str1[1] = 0x8A;
	utf8str1[2] = 0xAF;
	utf8str1[3] = 0x8D;
	utf8str1[4] = '\0';
	utf8ptr2 = utf8str2;
	LJ_UNITTEST_EQUALS( LJ_strWriteUTF8( &utf8ptr2, 0x10ABCD ), 4 );
	utf8str2[4] = '\0';
	LJ_UNITTEST_TRUE( LJ_strIsSame( utf8str1, utf8str2 ) );

	// In principal should test 5 and 6 byte utf8 codes but can't find any simple examples
}
LJ_UNITTEST_FUNCTION_END( str, utf8 )

LJ_UNITTEST_FUNCTION_START( str, misc )
{
	LJ_UNITTEST_TRUE( LJ_FALSE );
#if 0
// copy the rest of string to buffer if the start matches compare, returns LJ_TRUE if it does match
LJ_bool LJ_strCopyAfterMatchIgnoreCase( const LJ_char* const string, const LJ_char* const compare, LJ_char* const buffer );

// is the first of string the same as the whole of compare
LJ_bool LJ_strIsFirstPartSame( const LJ_char* const string, const LJ_char* const compare );
LJ_bool LJ_strIsFirstPartSameIgnoreCase( const LJ_char* const string, const LJ_char* const compare );

// if the first part of string matches stringToSkip, skip past it
LJ_bool LJ_strSkipString( const LJ_char** string, const LJ_char* const stringToSkip );
LJ_bool LJ_strSkipStringIgnoreCase( const LJ_char** string, const LJ_char* const stringToSkip );
#endif // #if 0
}
LJ_UNITTEST_FUNCTION_END( str, misc )

LJ_UNITTEST_FUNCTION_START( str, character )
{
	LJ_char buffer1[LJ_UNITTEST_STR_MAX_LEN];

	LJ_COMPILE_TIME_ASSERT( LJ_UNITTEST_STR_MAX_LEN > 21 );

	// The LJ_mem tests have already been done so I can trust the LJ_mem functions
	LJ_memZero( buffer1, LJ_UNITTEST_STR_MAX_LEN );
	LJ_strCopy( buffer1, "jake" );

	LJ_UNITTEST_TRUE( LJ_strIsSlash( '/' ) );
	LJ_UNITTEST_TRUE( LJ_strIsSlash( '\\' ) );
	LJ_UNITTEST_TRUE( ( LJ_strIsSlash( 'A' ) == LJ_FALSE ) );

	LJ_UNITTEST_TRUE( LJ_strIsAlpha( 'a' ) );
	LJ_UNITTEST_TRUE( LJ_strIsAlpha( 'M' ) );
	LJ_UNITTEST_TRUE( ( LJ_strIsAlpha( '0' ) == LJ_FALSE ) );

	LJ_UNITTEST_TRUE( LJ_strIsUpper( 'D' ) );
	LJ_UNITTEST_TRUE( LJ_strIsUpper( 'K' ) );
	LJ_UNITTEST_TRUE( ( LJ_strIsUpper( 'd' ) == LJ_FALSE ) );

	LJ_UNITTEST_TRUE( LJ_strIsLower( 'o' ) );
	LJ_UNITTEST_TRUE( LJ_strIsLower( 'j' ) );
	LJ_UNITTEST_TRUE( ( LJ_strIsLower( 'O' ) == LJ_FALSE ) );

	LJ_UNITTEST_TRUE( LJ_strIsDigit( '3' ) );
	LJ_UNITTEST_TRUE( LJ_strIsDigit( '8' ) );
	LJ_UNITTEST_TRUE( ( LJ_strIsDigit( 'G' ) == LJ_FALSE ) );

	LJ_UNITTEST_TRUE( LJ_strIsHexDigit( '3' ) );
	LJ_UNITTEST_TRUE( LJ_strIsHexDigit( 'D' ) );
	LJ_UNITTEST_TRUE( ( LJ_strIsHexDigit( 'G' ) == LJ_FALSE ) );

	LJ_UNITTEST_TRUE( ( LJ_strGetLastChar( buffer1 ) == 'e' ) );
	LJ_strSetLastChar( buffer1, '*' );
	LJ_UNITTEST_TRUE( ( LJ_strGetLastChar( buffer1 ) == '*' ) );
	LJ_strAppendChar( buffer1, '@' );
	LJ_UNITTEST_TRUE( ( LJ_strGetLastChar( buffer1 ) == '@' ) );
}
LJ_UNITTEST_FUNCTION_END( str, character )

LJ_UNITTEST_FUNCTION_START( str, slash )
{
	LJ_UNITTEST_TRUE( LJ_FALSE );
#if 0
// is the last character of the string a slash
LJ_bool LJ_strIsLastCharSlash( const LJ_char* const string );

// return '/' or '\\', depending on which is used by the string.
// if neither or both are used, returns defaultSlash
LJ_char LJ_strGetSlashToUse( const LJ_char* const string, const LJ_char defaultSlash );

// convert slashes
LJ_int LJ_strConvertSlashes( LJ_char* const string, const LJ_char slash );

// skip slashes
LJ_char* LJ_strSkipSlashes( const LJ_char* const string );

// remove slashes from the end of the string
void LJ_strRemoveSlashesFromEnd( LJ_char* const string );

// make sure there is a slash on the end of the string
void LJ_strMakeSureEndsWithSlash( LJ_char* const string, const LJ_char slash );

// find the first / last slash, returns LJ_NULL if no slashes
LJ_char* LJ_strFindFirstSlash( const LJ_char* const string );
LJ_char* LJ_strFindLastSlash( const LJ_char* const string );

// remove the rest of the string after the last slash, returns LJ_TRUE if anything removed
LJ_bool LJ_strRemoveAfterLastSlash( LJ_char* const string );

// does the string contain a slash
LJ_bool LJ_strHasSlash( const LJ_char* const string );
#endif
}
LJ_UNITTEST_FUNCTION_END( str, slash )

#endif // #if LJ_USE_UNITTEST

