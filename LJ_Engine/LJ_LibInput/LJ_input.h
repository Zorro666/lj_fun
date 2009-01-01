#ifndef LJ_INPUT_HH
#define LJ_INPUT_HH

///////////////////////////////////////////////////////////////////////////////////
//
// Standard module functions
//
///////////////////////////////////////////////////////////////////////////////////

void LJ_inputInit( void );
void LJ_inputReset( void );
void LJ_inputTick( void );
void LJ_inputShutdown( void );

///////////////////////////////////////////////////////////////////////////////////
//
// Keyboard support
//
///////////////////////////////////////////////////////////////////////////////////

typedef enum LJ_inputKeyEnum LJ_inputKey;
typedef enum LJ_inputKeyModifierEnum LJ_inputKeyModifier;

int LJ_inputKeyPressed( const LJ_inputKey key );
int LJ_inputKeyReleased( const LJ_inputKey key );
int LJ_inputKeyClicked( const LJ_inputKey key );

char* const LJ_inputGetKeyName( const LJ_inputKey key );

// the state of the modifier keys e.g. shift, control, alt, etc.
LJ_inputKeyModifier LJ_inputGetKeyModifier( void );

///////////////////////////////////////////////////////////////////////////////////
//
// Mouse support
//
///////////////////////////////////////////////////////////////////////////////////

typedef enum LJ_inputMouseButtonEnum LJ_inputMouseButton;

void LJ_inputMouseGetPosition( int* const x, int* const y );
void LJ_inputMouseGetMovement( int* const deltaX, int* const deltaY );
int LJ_inputMouseButtonPressed( const LJ_inputMouseButton button );
int LJ_inputMouseButtonReleased( const LJ_inputMouseButton button );
int LJ_inputMouseButtonClicked( const LJ_inputMouseButton button );

#endif // #ifndef LJ_INPUT_HH

