#include "LJ.h"

#include "LJ_File/LJ_filesystem.h"

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

LJ_fileHandle LJ_filesystemOpen( const LJ_char* const filename, const LJ_uint mode );

LJ_bool LJ_filesystemRead( const LJ_fileHandle fileHandle );
LJ_bool LJ_filesystemWrite( const LJ_fileHandle fileHandle );
LJ_bool LJ_filesystemClose( const LJ_fileHandle fileHandle );

LJ_int LJ_filesystemGetSize( const LJ_char* const filename );

LJ_bool LJ_filesystemLoadFile( const LJ_char* const filename, LJ_memHeapHandle memHeap );

