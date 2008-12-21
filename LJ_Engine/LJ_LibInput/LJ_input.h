#ifndef LJ_INPUT_HH
#define LJ_INPUT_HH

void LJ_inputInit( void );
void LJ_inputReset( void );
void LJ_inputShutdown( void );

typedef enum LJ_inputKeyEnum LJ_inputKey;
typedef enum LJ_inputKeyModifierEnum LJ_inputKeyModifier;

int LJ_inputIsKeyPressed( const LJ_inputKey key );
int LJ_inputIsKeyRelease( const LJ_inputKey key );
int LJ_inputKeyClicked( const LJ_inputKey key );

#endif // #ifndef LJ_INPUT_HH

