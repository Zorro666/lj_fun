#ifndef LJ_INPUT_HH
#define LJ_INPUT_HH

void LJ_inputInit( void );
void LJ_inputReset( void );
void LJ_inputTick( void );
void LJ_inputShutdown( void );

typedef enum LJ_inputKeyEnum LJ_inputKey;
typedef enum LJ_inputKeyModifierEnum LJ_inputKeyModifier;

int LJ_inputKeyPressed( const LJ_inputKey key );
int LJ_inputKeyReleased( const LJ_inputKey key );
int LJ_inputKeyClicked( const LJ_inputKey key );

LJ_inputKeyModifier LJ_inputGetKeyModifier( void );

char* const LJ_inputGetKeyName( const LJ_inputKey key );

#endif // #ifndef LJ_INPUT_HH

