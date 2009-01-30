#ifndef LJ_FILE_H
#define LJ_FILE_H

// internal dependencies
#include "LJ_LibCommon/LJ_Base/LJ_types.h"
#include "LJ_LibCommon/LJ_File/LJ_filehandle.h"

void LJ_fileInit( void );
void LJ_fileReset( void );
void LJ_fileShutdown( void );

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: low-level file API used by the LJ_filesystem API (do not call these functions directly)
//
////////////////////////////////////////////////////////////////////////////////////////////////////

LJ_fileHandle LJ_fileOpen( const LJ_char* const filename, const LJ_fileMode mode );

LJ_bool LJ_fileRead( const LJ_fileHandle fileHandle, void* const dest, const LJ_uint numBytes );
LJ_bool LJ_fileWrite( const LJ_fileHandle fileHandle, const void* const src, const LJ_uint numBytes );
LJ_bool LJ_fileClose( const LJ_fileHandle fileHandle );

LJ_int LJ_fileGetSize( const LJ_char* const filename );

#endif // #ifndef LJ_FILE_H

