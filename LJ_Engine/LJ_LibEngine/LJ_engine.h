#ifndef LJ_ENGINE_HH
#define LJ_ENGINE_HH

void LJ_engineInit( int argc, char* argv[] );

void LJ_engineReset( void );

void LJ_engineShutdown( void );

void LJ_engineTick( void );

void LJ_engineStartRendering( void );
void LJ_engineRender( void );
void LJ_engineFinishRendering( void );

void LJ_debugDrawSphere( float x, float y, float z, float radius, int colour );
void LJ_debugDrawCircle( float x, float y, float z, float radius, int colour );

#endif // #ifndef LJ_ENGINE_HH

