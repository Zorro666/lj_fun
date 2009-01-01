#ifndef LJ_ENGINE_HH
#define LJ_ENGINE_HH

void LJ_engineInit( LJ_int argc, LJ_char* argv[] );

void LJ_engineReset( void );

void LJ_engineShutdown( void );

void LJ_engineTick( void );

void LJ_engineStartRendering( void );
void LJ_engineRender( void );
void LJ_engineFinishRendering( void );

void LJ_debugDrawSphere( LJ_float x, LJ_float y, LJ_float z, LJ_float radius, LJ_uint colour );
void LJ_debugDrawCircle( LJ_float x, LJ_float y, LJ_float z, LJ_float radius, LJ_uint colour );

#endif // #ifndef LJ_ENGINE_HH

