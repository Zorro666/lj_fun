#ifndef LJ_TEXTURE_H
#define LJ_TEXTURE_H

// internal dependencies
#include "LJ_LibCommon/LJ_Base/LJ_types.h"

void LJ_textureInit( void );
void LJ_textureReset( void );
void LJ_textureShutdown( void );

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: 
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#define LJ_TEXTURE_HANDLE_INVALID (0xFFFFFFFFU)
typedef LJ_uint LJ_textureHandle;

LJ_textureHandle LJ_textureLoadTGA( const char* const filename, LJ_uint8** imageDataPtr );
LJ_bool LJ_textureFreeTexture( const LJ_textureHandle texHandle );

#endif // #ifndef LJ_TEXTURE_H

