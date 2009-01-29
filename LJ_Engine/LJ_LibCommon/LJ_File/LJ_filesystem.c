#include "LJ.h"

#include "LJ_File/LJ_filesystem.h"
#include "LJ_File/LJ_file.h"

void LJ_filesystemInit( void )
{
}

void LJ_filesystemReset( void )
{
}

void LJ_filesystemShutdown( void )
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: high-level file API - open, read, write, close, getsize 
//
////////////////////////////////////////////////////////////////////////////////////////////////////

LJ_fileHandle LJ_filesystemOpen( const LJ_char* const filename, const LJ_fileMode mode )
{
    LJ_fileHandle fh = LJ_FILE_HANDLE_INVALID;

    fh = LJ_fileOpen( filename, mode );

    return fh;
}

LJ_bool LJ_filesystemRead( const LJ_fileHandle fileHandle, void* const dest, const LJ_uint numBytes )
{
    LJ_bool retVal = LJ_FALSE;

    if ( fileHandle == LJ_FILE_HANDLE_INVALID )
    {
	return LJ_FALSE;
    }

    retVal = LJ_fileRead( fileHandle, dest, numBytes );

    return retVal;
}

LJ_bool LJ_filesystemWrite( const LJ_fileHandle fileHandle, const void* const src, const LJ_uint numBytes )
{
    LJ_bool retVal = LJ_FALSE;

    if ( fileHandle == LJ_FILE_HANDLE_INVALID )
    {
	return LJ_FALSE;
    }

    retVal = LJ_fileWrite( fileHandle, src, numBytes );

    return retVal;
}

LJ_bool LJ_filesystemClose( const LJ_fileHandle fileHandle )
{
    LJ_bool retVal = LJ_FALSE;

    if ( fileHandle == LJ_FILE_HANDLE_INVALID )
    {
	return LJ_FALSE;
    }

    retVal = LJ_fileClose( fileHandle );

    return retVal;
}

LJ_int LJ_filesystemGetSize( const LJ_char* const filename )
{
    LJ_int retVal = -1;

    retVal = LJ_fileGetSize( filename );

    return retVal;
}

// TODO LJ_bool LJ_filesystemLoadFile( const LJ_char* const filename, LJ_memHeapHandle memHeap );

