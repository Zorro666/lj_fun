#ifndef LJ_ENGINE_H
#define LJ_ENGINE_H

#include "LJ_LibCommon/LJ_LibCommon.h"

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

#endif // #ifndef LJ_ENGINE_H

