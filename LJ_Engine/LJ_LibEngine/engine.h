#ifndef LJ_ENGINE_HH
#define LJ_ENGINE_HH

void engineInit( int argc, char* argv[] );

void engineReset( void );

void engineShutdown( void );

void engineTick( void );

void engineRender( void );

void engineHandleInput( void );

float debugVarRenderText( const int render, const float x, const float y, const unsigned int colour, const char* const outputString );

enum
{
	KEY_ESCAPE = 27,
	KEY_BACKSPACE = 8,
	KEY_UP = 273,
	KEY_DOWN = 274,
	KEY_RIGHT = 275,
	KEY_LEFT = 276,
};

#endif // #ifndef LJ_ENGINE_HH

