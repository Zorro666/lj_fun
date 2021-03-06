#ifndef LJ_ENGINE_H
#define LJ_ENGINE_H

#include "LJ_LibCommon/LJ_LibCommon.h"
#include "LJ_LibGraphics/LJ_LibGraphics.h"

void LJ_engineEarlyInit( LJ_int argc, LJ_char* argv[] );
void LJ_engineLateInit( void );
void LJ_engineReset( void );
void LJ_engineShutdown( void );
void LJ_engineTick( void );

void LJ_engineStartRendering( void );
void LJ_engineRender( void );
void LJ_engineFinishRendering( void );

void LJ_debugDrawSphere( const LJ_float x, const LJ_float y, const LJ_float z, const LJ_float radius, const LJ_uint colour );
void LJ_debugDrawCircle( const LJ_float x, const LJ_float y, const LJ_float z, const LJ_float radius, const LJ_uint colour );
void LJ_debugDrawQuadTexture( const LJ_float x, const LJ_float y, const LJ_float z, 
						      const LJ_float width, const LJ_float height, const LJ_textureHandle th, const LJ_uint colour );

extern void game3DRender( void );
extern void game2DRender( void );
extern void gameExitFunction( void );

#endif // #ifndef LJ_ENGINE_H

