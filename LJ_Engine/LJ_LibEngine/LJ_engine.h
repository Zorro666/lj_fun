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

enum
{
	LJ_KEY_ESCAPE = 27,
	LJ_KEY_BACKSPACE = 8,
	LJ_KEY_UP = 273,
	LJ_KEY_DOWN = 274,
	LJ_KEY_RIGHT = 275,
	LJ_KEY_LEFT = 276,
};

#endif // #ifndef LJ_ENGINE_HH

