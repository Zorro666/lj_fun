#ifndef LJ_GRAPHICS_H
#define LJ_GRAPHICS_H

// internal dependencies
#include "LJ_LibCommon/LJ_Base/LJ_types.h"

void LJ_graphicsInit( void );
void LJ_graphicsReset( void );
void LJ_graphicsShutdown( void );

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: very simple camera functions
//
/////////////////////////////////////////////////////////////////////////////////////////////////

void LJ_cameraSetOrthographic( const LJ_float midX, const LJ_float midY, const LJ_float width, const LJ_float height );

#endif // #ifndef LJ_GRAPHICS_H

