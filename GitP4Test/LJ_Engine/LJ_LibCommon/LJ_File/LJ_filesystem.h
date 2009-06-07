#ifndef LJ_FILESYSTEM_H
#define LJ_FILESYSTEM_H

// internal dependencies
#include "LJ_LibCommon/LJ_Base/LJ_types.h"
#include "LJ_LibCommon/LJ_Mem/LJ_mem.h"
#include "LJ_LibCommon/LJ_File/LJ_filehandle.h"

void LJ_filesystemInit( void );
void LJ_filesystemReset( void );
void LJ_filesystemShutdown( void );

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: high-level file API - open, read, write, close, getsize etc.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

LJ_fileHandle LJ_filesystemOpen( const LJ_char* const filename, const LJ_fileMode mode );

LJ_bool LJ_filesystemRead( const LJ_fileHandle fileHandle, void* const dest, const LJ_uint numBytes );
LJ_bool LJ_filesystemWrite( const LJ_fileHandle fileHandle, const void* const src, const LJ_uint numBytes );
LJ_bool LJ_filesystemClose( const LJ_fileHandle fileHandle );

LJ_int LJ_filesystemGetSize( const LJ_char* const filename );

// TODO LJ_bool LJ_filesystemLoadFile( const LJ_char* const filename, LJ_memHeapHandle memHeap );

#endif // #ifndef LJ_FILESYSTEM_H

