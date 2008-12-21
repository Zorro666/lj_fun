#ifndef LJ_INPUT_HH
#define LJ_INPUT_HH

void LJ_inputInit( void );
void LJ_inputReset( void );
void LJ_inputShutdown( void );

enum LJ_inputKeyEnum
{
	LJ_KEY_ESCAPE = 27,
	LJ_KEY_BACKSPACE = 8,
	LJ_KEY_UP = 273,
	LJ_KEY_DOWN = 274,
	LJ_KEY_RIGHT = 275,
	LJ_KEY_LEFT = 276,
};

typedef enum LJ_inputKeyEnum LJ_inputKey;

int LJ_inputIsKeyPressed( const LJ_inputKey key );
int LJ_inputIsKeyRelease( const LJ_inputKey key );
int LJ_inputKeyClicked( const LJ_inputKey key );

#endif // #ifndef LJ_INPUT_HH

