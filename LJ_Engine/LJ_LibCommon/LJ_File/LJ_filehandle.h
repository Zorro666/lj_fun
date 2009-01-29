#ifndef LJ_FILEHANDLE_H
#define LJ_FILEHANDLE_H

// internal dependencies
#include "LJ_LibCommon/LJ_Base/LJ_types.h"

#define LJ_FILE_HANDLE_INVALID		(0xFFFFFFFFU)

typedef LJ_uint LJ_fileHandle;

#define LJ_FILE_MODE_READ		(1 << 0)
#define LJ_FILE_MODE_WRITE		(1 << 1)
#define LJ_FILE_MODE_APPEND		(1 << 2)

typedef LJ_uint LJ_fileMode;

#endif // #ifndef LJ_FILEHANDLE_H

