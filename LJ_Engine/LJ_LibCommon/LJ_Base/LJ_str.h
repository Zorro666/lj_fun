#ifndef LJ_STR_H
#define LJ_STR_H

// internal dependencies
#include "LJ_types.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: Provides a suite of replacement functions for stdlib string functions.
//
/////////////////////////////////////////////////////////////////////////////////////////////////

LJ_uint LJ_strGetLength( const LJ_char* const string );

void LJ_strCopy( LJ_char* const to, const LJ_char* const from );
void LJ_strNCopy( LJ_char* const to, const LJ_char* const from, const LJ_uint maxLength );

LJ_bool LJ_strIsSame( const LJ_char* const string, const LJ_char* const compare );
LJ_bool LJ_strIsNSame( const LJ_char* const string, const LJ_char* const compare, const LJ_uint maxLength );

LJ_bool LJ_strIsSameIgnoreCase( const LJ_char* const string, const LJ_char* const compare );
LJ_bool LJ_strIsNSameIgnoreCase( const LJ_char* const string, const LJ_char* const compare, const LJ_uint maxLength );

void LJ_strToUpper( LJ_char* const to, const LJ_char* const from );
void LJ_strToLower( LJ_char* const to, const LJ_char* const from );

LJ_char LJ_strToUpperChar( const LJ_char c );
LJ_char LJ_strToLowerChar( const LJ_char c );

LJ_int LJ_strFindChar( const LJ_char* const string, const LJ_char c );
LJ_int LJ_strReverseFindChar( const LJ_char* const string, const LJ_char c );

void LJ_strCat( LJ_char* const to, const LJ_char* const from );
void LJ_strCatChar( LJ_char* const to, const LJ_char c );
void LJ_strNCat( LJ_char* const to, const LJ_char* const from, const LJ_uint maxLength );

LJ_float LJ_strToFloat( const LJ_char* const string );
LJ_int LJ_strToInt( const LJ_char* const string );
LJ_int LJ_strVSPrint( LJ_char* const to, const LJ_uint maxLength, const LJ_char* const format, LJ_valist* list );
LJ_int LJ_strSPrint( LJ_char* const to, const LJ_uint maxLength, const LJ_char* const format, ... );
LJ_bool LJ_strIsHex( const LJ_char* const string, LJ_uint* const val );
LJ_bool LJ_strIsInt( const LJ_char* const string, LJ_int* const val );

LJ_int LJ_strFindTextIndex( const LJ_char* const string, const LJ_char* const find );
LJ_int LJ_strFindTextIgnoreCaseIndex( const LJ_char* const string, const LJ_char* const find );
LJ_bool LJ_strFindText( const LJ_char* const string, const LJ_char* const find );
LJ_bool LJ_strFindTextIgnoreCase( const LJ_char* const string, const LJ_char* const find );

// -1 if string < compare, 0 if string = compare, 1 if string > compare
LJ_int LJ_strCompare( const LJ_char* const string, const LJ_char* const compare );

LJ_uint LJ_strReadUTF8( const LJ_char** c );
LJ_uint LJ_strWriteUTF8( LJ_char** c, const LJ_uint unicode );

// copy the rest of string to buffer if the start matches compare, returns LJ_TRUE if it does match
LJ_bool LJ_strCopyAfterMatchIgnoreCase( const LJ_char* const string, const LJ_char* const compare, LJ_char* const buffer );

// is the first of string the same as the whole of compare
LJ_bool LJ_strIsFirstPartSame( const LJ_char* const string, const LJ_char* const compare );
LJ_bool LJ_strIsFirstPartSameIgnoreCase( const LJ_char* const string, const LJ_char* const compare );

// if the first part of string matches stringToSkip, skip past it
LJ_bool LJ_strSkipString( const LJ_char** string, const LJ_char* const stringToSkip );
LJ_bool LJ_strSkipStringIgnoreCase( const LJ_char** string, const LJ_char* const stringToSkip );

//character comparisons
LJ_EXTERN_INLINE LJ_bool LJ_strIsSlash( const LJ_char c );				// is either slash?
LJ_EXTERN_INLINE LJ_bool LJ_strIsAlpha( const LJ_char c );				// is a-z or A-Z
LJ_EXTERN_INLINE LJ_bool LJ_strIsUpper( const LJ_char c );				// is uppercase A-Z?
LJ_EXTERN_INLINE LJ_bool LJ_strIsLower( const LJ_char c );				// is lowercase a-z?
LJ_EXTERN_INLINE LJ_bool LJ_strIsDigit( const LJ_char c );				// is 0-9
LJ_EXTERN_INLINE LJ_bool LJ_strIsHexDigit( const LJ_char c );			// is 0-9 or A-F

LJ_char LJ_strGetLastChar( const LJ_char* const string );
void LJ_strSetLastChar( LJ_char* const string, const LJ_char c );
void LJ_strAppendChar( LJ_char* const string, const LJ_char c );

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

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Inline functions
//
/////////////////////////////////////////////////////////////////////////////////////////////////

// is either slash?
LJ_EXTERN_INLINE LJ_bool LJ_strIsSlash( const LJ_char c )
{
	return( (c == '\\') || (c == '/') );
}

LJ_EXTERN_INLINE LJ_bool LJ_strIsAlpha( const LJ_char c )
{
	return( ((c >= 'A') && (c <= 'Z')) ||
			((c >= 'a') && (c <= 'z')) );
}

LJ_EXTERN_INLINE LJ_bool LJ_strIsUpper( const LJ_char c )
{
	return( (c >= 'A') && (c <= 'Z') );
}

LJ_EXTERN_INLINE LJ_bool LJ_strIsLower( const LJ_char c )
{
	return( (c >= 'a') && (c <= 'z') );
}

LJ_EXTERN_INLINE LJ_bool LJ_strIsDigit( const LJ_char c )
{
	return( (c >= '0') && (c <= '9') );
}

LJ_EXTERN_INLINE LJ_bool LJ_strIsHexDigit( const LJ_char c )
{
	return( ((c >= '0') && (c <= '9')) ||
			((c >= 'A') && (c <= 'F')) ||
			((c >= 'a') && (c <= 'f')) );
}

#endif // #ifndef LJ_STR_H
