#ifndef LJ_FILE_H
#define LJ_FILE_H

// internal dependencies
#include "LJ_LibCommon/LJ_Base/LJ_types.h"

void LJ_fileInit( void );
void LJ_fileReset( void );
void LJ_fileShutdown( void );

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: low-level file API used by the LJ_filesystem API (do not call these functions directly)
//
////////////////////////////////////////////////////////////////////////////////////////////////////

typedef LJ_uint LJ_fileHandle;

#endif // #ifndef LJ_FILE_H

