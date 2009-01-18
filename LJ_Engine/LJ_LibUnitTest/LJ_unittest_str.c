#include "LJ.h"
#include "LJ_unittest.h"
#include "LJ_LibCommon/LJ_LibCommon.h"

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
	LJ_UNITTEST_FLOAT_EQUALS( LJ_strToFloat( "0.123456jake" ), 0.123456f, 1.0e-6f );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_strToFloat( "-0.123456" ), -0.123456f, 1.0e-6f );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_strToFloat( "123.456" ), 123.456f, 1.0e-5f );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_strToFloat( "123.456e-3" ), 123.456e-3f, 1.0e-6f );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_strToFloat( "0.123456e-3" ), 0.123456e-3f, 1.0e-6f );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_strToFloat( "0.123456e+02" ), 0.123456e+02f, 1.0e-6f );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_strToFloat( "1234.56e+02" ), 1234.56e+02f, 1.0e-6f );
	LJ_UNITTEST_FLOAT_EQUALS( LJ_strToFloat( "0.123456e+10" ), 0.123456e+10f, 1.0e-6f );

	LJ_UNITTEST_EQUALS( LJ_strToInt( "    123456" ), +123456 );
	LJ_UNITTEST_EQUALS( LJ_strToInt( "    +123456" ), +123456 );
	LJ_UNITTEST_EQUALS( LJ_strToInt( "    -123456" ), -123456 );
	LJ_UNITTEST_EQUALS( LJ_strToInt( "    +0xABCD" ), +0xABCD );
	LJ_UNITTEST_EQUALS( LJ_strToInt( "    -0xABCD" ), -0xABCD );
	LJ_UNITTEST_EQUALS( LJ_strToInt( "    0123" ), 0123 );
	LJ_UNITTEST_EQUALS( LJ_strToInt( "    -0123" ), -0123 );
	LJ_UNITTEST_EQUALS( LJ_strToInt( "-0123" ), -0123 );
	LJ_UNITTEST_EQUALS( LJ_strToInt( "   +03201" ), 03201 );

	LJ_UNITTEST_TRUE( LJ_strIsHex( "0xDEADBEAF", &hexVal ) );
	LJ_UNITTEST_EQUALS( hexVal, 0xDEADBEAF );
	LJ_UNITTEST_TRUE( LJ_strIsInt( "123456", &intVal ) );
	LJ_UNITTEST_TRUE( LJ_strIsInt( "+123456", &intVal ) );
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
	LJ_char buffer1[LJ_UNITTEST_STR_MAX_LEN];
	LJ_char buffer2[LJ_UNITTEST_STR_MAX_LEN];
	const LJ_char* bufferPtr1;

	LJ_COMPILE_TIME_ASSERT( LJ_UNITTEST_STR_MAX_LEN > 21 );

	// The LJ_mem tests have already been done so I can trust the LJ_mem functions
	LJ_memZero( buffer1, LJ_UNITTEST_STR_MAX_LEN );
	LJ_memZero( buffer2, LJ_UNITTEST_STR_MAX_LEN );

	LJ_strCopy( buffer1, "ABCDEFGHIJKLMNOPQRST" );

	LJ_UNITTEST_TRUE( LJ_strCopyAfterMatchIgnoreCase( buffer1, "aBcDeF", buffer2 ) );
	LJ_UNITTEST_TRUE( LJ_strIsSame( buffer2, "GHIJKLMNOPQRST" ) );

	LJ_UNITTEST_TRUE( LJ_strCopyAfterMatchIgnoreCase( buffer1, "JAKE", buffer2 ) == LJ_FALSE );

	LJ_UNITTEST_TRUE( LJ_strIsFirstPartSame( buffer1, "ABCDEF" ) );
	LJ_UNITTEST_TRUE( LJ_strIsFirstPartSame( buffer1, "Mufasa" ) == LJ_FALSE );

	LJ_UNITTEST_TRUE( LJ_strIsFirstPartSameIgnoreCase( buffer1, "aBcdEF" ) );
	LJ_UNITTEST_TRUE( LJ_strIsFirstPartSameIgnoreCase( buffer1, "Double" ) == LJ_FALSE );

	bufferPtr1 = buffer1;
	LJ_UNITTEST_TRUE( LJ_strSkipString( &bufferPtr1, "ABCDEF" ) );
	LJ_UNITTEST_TRUE( LJ_strIsSame( bufferPtr1, "GHIJKLMNOPQRST" ) );
	LJ_UNITTEST_TRUE( LJ_strSkipString( &bufferPtr1, "abcdef" ) == LJ_FALSE );

	bufferPtr1 = buffer1;
	LJ_UNITTEST_TRUE( LJ_strSkipStringIgnoreCase( &bufferPtr1, "aBcDeF" ) );
	LJ_UNITTEST_TRUE( LJ_strIsSame( bufferPtr1, "GHIJKLMNOPQRST" ) );
	LJ_UNITTEST_TRUE( LJ_strSkipStringIgnoreCase( &bufferPtr1, "Single" ) == LJ_FALSE );
}
LJ_UNITTEST_FUNCTION_END( str, misc )

LJ_UNITTEST_FUNCTION_START( str, character )
{
	LJ_char buffer1[LJ_UNITTEST_STR_MAX_LEN];
	LJ_char* bufferPtr1;

	LJ_COMPILE_TIME_ASSERT( LJ_UNITTEST_STR_MAX_LEN > 21 );

	// The LJ_mem tests have already been done so I can trust the LJ_mem functions
	LJ_memZero( buffer1, LJ_UNITTEST_STR_MAX_LEN );
	LJ_strCopy( buffer1, "jake" );

	LJ_UNITTEST_TRUE( LJ_strIsWhiteSpace( ' ' ) );
	LJ_UNITTEST_TRUE( LJ_strIsWhiteSpace( '\t' ) );
	LJ_UNITTEST_TRUE( LJ_strIsWhiteSpace( '\n' ) );
	LJ_UNITTEST_TRUE( LJ_strIsWhiteSpace( '\r' ) );
	LJ_UNITTEST_TRUE( ( LJ_strIsWhiteSpace( 'A' ) == LJ_FALSE ) );
	LJ_UNITTEST_TRUE( ( LJ_strIsWhiteSpace( '0' ) == LJ_FALSE ) );
	LJ_UNITTEST_TRUE( ( LJ_strIsWhiteSpace( 'a' ) == LJ_FALSE ) );
	LJ_UNITTEST_TRUE( ( LJ_strIsWhiteSpace( 'g' ) == LJ_FALSE ) );

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

	LJ_strCopy( buffer1, "OhNoJake" );
	bufferPtr1 = LJ_strEatWhiteSpace( buffer1 );
	LJ_UNITTEST_TRUE( ( LJ_strIsSame( bufferPtr1, "OhNoJake" ) ) );

	LJ_strCopy( buffer1, "  	\n\r	OhNo Jake" );
	bufferPtr1 = LJ_strEatWhiteSpace( buffer1 );
	LJ_UNITTEST_TRUE( ( LJ_strIsSame( bufferPtr1, "OhNo Jake" ) ) );
}
LJ_UNITTEST_FUNCTION_END( str, character )

LJ_UNITTEST_FUNCTION_START( str, slash )
{
	LJ_char buffer1[LJ_UNITTEST_STR_MAX_LEN];
	LJ_char* bufferPtr1;

	LJ_COMPILE_TIME_ASSERT( LJ_UNITTEST_STR_MAX_LEN > 21 );

	// The LJ_mem tests have already been done so I can trust the LJ_mem functions
	LJ_memZero( buffer1, LJ_UNITTEST_STR_MAX_LEN );

	LJ_strCopy( buffer1, "ABCDEFGHIJKLMNOPQRST" );

	LJ_UNITTEST_TRUE( LJ_strIsLastCharSlash( "OhYesItIs/" ) );
	LJ_UNITTEST_TRUE( LJ_strIsLastCharSlash( "OhYesItIs\\" ) );
	LJ_UNITTEST_TRUE( LJ_strIsLastCharSlash( "OhNoItsNot/No" ) == LJ_FALSE );

	LJ_UNITTEST_EQUALS( LJ_strGetSlashToUse( "OhNoItsNot/No", '/' ), '/' );
	LJ_UNITTEST_EQUALS( LJ_strGetSlashToUse( "OhNoItsNot", '/' ), '/' );
	LJ_UNITTEST_EQUALS( LJ_strGetSlashToUse( "OhNoItsNot\\Jake", '/' ), '\\' );
	LJ_UNITTEST_EQUALS( LJ_strGetSlashToUse( "OhNo/ItsNot\\Jake", '\\' ), '\\' );
	LJ_UNITTEST_EQUALS( LJ_strGetSlashToUse( "OhNo/ItsNot\\Jake", '/' ), '/' );

	LJ_strCopy( buffer1, "OhNoJake" );
	LJ_UNITTEST_EQUALS( LJ_strConvertSlashes( buffer1, '/' ), 0 );
	LJ_strCopy( buffer1, "OhN\\oJake" );
	LJ_UNITTEST_EQUALS( LJ_strConvertSlashes( buffer1, '/' ), 1 );
	LJ_UNITTEST_TRUE( LJ_strIsSame( buffer1, "OhN/oJake" ) );
	LJ_strCopy( buffer1, "OhN/oJ/ak\\e" );
	LJ_UNITTEST_EQUALS( LJ_strConvertSlashes( buffer1, '/' ), 3 );
	LJ_UNITTEST_TRUE( LJ_strIsSame( buffer1, "OhN/oJ/ak/e" ) );

	LJ_strCopy( buffer1, "\\OhN\\oJake" );
	bufferPtr1 = LJ_strSkipSlashes( buffer1 );
	LJ_UNITTEST_TRUE( LJ_strIsSame( bufferPtr1, "OhN\\oJake" ) );

	LJ_strCopy( buffer1, "\\OhN\\oJake//\\" );
	LJ_strRemoveSlashesFromEnd( buffer1 );
	LJ_UNITTEST_TRUE( LJ_strIsSame( buffer1, "\\OhN\\oJake" ) );

	LJ_strCopy( buffer1, "\\OhN\\oJake" );
	LJ_strMakeSureEndsWithSlash( buffer1, '/' );
	LJ_UNITTEST_TRUE( LJ_strIsSame( buffer1, "\\OhN\\oJake/" ) );

	LJ_strCopy( buffer1, "\\OhN\\oJake" );
	LJ_strMakeSureEndsWithSlash( buffer1, '\\' );
	LJ_UNITTEST_TRUE( LJ_strIsSame( buffer1, "\\OhN\\oJake\\" ) );

	LJ_strCopy( buffer1, "\\OhN\\oJake" );
	LJ_strMakeSureEndsWithSlash( buffer1, '#' );
	LJ_UNITTEST_TRUE( LJ_strIsSame( buffer1, "\\OhN\\oJake#" ) );

	LJ_strCopy( buffer1, "OhN\\oJake" );
	bufferPtr1 = LJ_strFindFirstSlash( buffer1 );
	LJ_UNITTEST_TRUE( LJ_strIsSame( bufferPtr1, "\\oJake" ) );

	LJ_strCopy( buffer1, "/OhN\\oJake" );
	bufferPtr1 = LJ_strFindFirstSlash( buffer1 );
	LJ_UNITTEST_TRUE( LJ_strIsSame( bufferPtr1, "/OhN\\oJake" ) );

	LJ_strCopy( buffer1, "OhNoJake" );
	bufferPtr1 = LJ_strFindFirstSlash( buffer1 );
	LJ_UNITTEST_EQUALS( bufferPtr1, LJ_NULL );

	LJ_strCopy( buffer1, "OhN\\oJak/e" );
	bufferPtr1 = LJ_strFindLastSlash( buffer1 );
	LJ_UNITTEST_TRUE( LJ_strIsSame( bufferPtr1, "/e" ) );

	LJ_strCopy( buffer1, "/OhN\\oJake" );
	bufferPtr1 = LJ_strFindLastSlash( buffer1 );
	LJ_UNITTEST_TRUE( LJ_strIsSame( bufferPtr1, "\\oJake" ) );

	LJ_strCopy( buffer1, "OhNoJake" );
	bufferPtr1 = LJ_strFindLastSlash( buffer1 );
	LJ_UNITTEST_EQUALS( bufferPtr1, LJ_NULL );

	LJ_strCopy( buffer1, "/OhN/oJake" );
	LJ_UNITTEST_TRUE( LJ_strRemoveAfterLastSlash( buffer1 ) );
	LJ_UNITTEST_TRUE( LJ_strIsSame( buffer1, "/OhN/" ) );

	LJ_strCopy( buffer1, "/OhNr\\oJake" );
	LJ_UNITTEST_TRUE( LJ_strRemoveAfterLastSlash( buffer1 ) );
	LJ_UNITTEST_TRUE( LJ_strIsSame( buffer1, "/OhNr\\" ) );

	LJ_strCopy( buffer1, "OhNoJake" );
	LJ_UNITTEST_TRUE( LJ_strHasSlash( buffer1 ) == LJ_FALSE );
	LJ_strCopy( buffer1, "OhN\\oJake" );
	LJ_UNITTEST_TRUE( LJ_strHasSlash( buffer1 ) );
	LJ_strCopy( buffer1, "OhN/oJ/ak\\e" );
	LJ_UNITTEST_TRUE( LJ_strHasSlash( buffer1 ) );
}
LJ_UNITTEST_FUNCTION_END( str, slash )

#endif // #if LJ_USE_UNITTEST

