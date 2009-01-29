#include "LJ.h"

#include "LJ_File/LJ_file.h"
#include "LJ_Base/LJ_output.h"

#include <stdio.h>

void LJ_fileInit( void )
{
}

void LJ_fileReset( void )
{
}

void LJ_fileShutdown( void )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: low-level file API used by the LJ_filesystem API (do not call these functions directly)
// This implementation will be different per platform and file API used e.g. std C file API 
//
////////////////////////////////////////////////////////////////////////////////////////////////////

// Std C implementation at the moment

LJ_fileHandle LJ_fileOpen( const LJ_char* const filename, const LJ_fileMode mode )
{
    FILE* file = LJ_NULL;
    LJ_fileHandle fh = LJ_FILE_HANDLE_INVALID;
    char modeStr[4];
    char* modeChar = modeStr;
    *modeChar = '\0';

    if ( mode & LJ_FILE_MODE_READ )
    {
	*modeChar = 'r';
	modeChar++;
    }
    else if ( mode & LJ_FILE_MODE_WRITE )
    {
	*modeChar = 'w';
	modeChar++;
    }
    else if ( mode & LJ_FILE_MODE_APPEND )
    {
	*modeChar = 'a';
	modeChar++;
    }
    *modeChar = '\0';

    if ( modeChar == modeStr )
    {
	// An invalid mode passed in
	LJ_outputPrintGold( ( "Warning: LJ_fileOpen '%s' invalid mode 0x%X\n", filename, mode ) );
	return LJ_FILE_HANDLE_INVALID;
    }

    file = fopen( filename, modeStr );
	if ( file == LJ_NULL )
	{
		fh = LJ_FILE_HANDLE_INVALID;
	}
	else
	{
		fh = (LJ_fileHandle)( file );
	}

    return fh;
}

LJ_bool LJ_fileRead( const LJ_fileHandle fileHandle, void* const dest, const LJ_uint numBytes )
{
    FILE* const file = (FILE*)( fileHandle );

    const LJ_uint numRead = fread( dest, 1, numBytes, file );

    if ( numRead != numBytes )
    {
	return LJ_FALSE;
    }

    return LJ_TRUE;
}

LJ_bool LJ_fileWrite( const LJ_fileHandle fileHandle, const void* const src, const LJ_uint numBytes )
{
    FILE* const file = (FILE*)( fileHandle );

    const LJ_uint numWritten = fwrite( src, 1, numBytes, file );

    if ( numWritten != numBytes )
    {
	return LJ_FALSE;
    }

    return LJ_TRUE;
}

LJ_bool LJ_fileClose( const LJ_fileHandle fileHandle )
{
    FILE* const file = (FILE*)( fileHandle );

    const LJ_int retVal = fclose( file );

    if ( retVal != 0 )
    {
	return LJ_FALSE;
    }

    return LJ_TRUE;
}

LJ_int LJ_fileGetSize( const LJ_char* const filename )
{
    LJ_fileHandle fh = LJ_FILE_HANDLE_INVALID;
    FILE* file = LJ_NULL;
    LJ_bool retVal = LJ_FALSE;
    LJ_int size = -1;
    
    fh = LJ_fileOpen( filename, LJ_FILE_MODE_READ );
    if ( fh == LJ_FILE_HANDLE_INVALID )
    {
	return -1;
    }
    file = (FILE*)( fh );

    size = fseek( file, 0, SEEK_END );
    if ( size != 0 )
    {
	LJ_fileClose( fh );
	return -1;
    }
    size = ftell( file );

    retVal = LJ_fileClose( fh );
    if ( retVal == LJ_FALSE )
    {
	return -1;
    }

    return size;
}

