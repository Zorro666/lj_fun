#ifndef LJ_INPUT_H
#define LJ_INPUT_H

#include "LJ_LibCommon/LJ_LibCommon.h"

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

LJ_bool LJ_inputKeyPressed( const LJ_inputKey key );
LJ_bool LJ_inputKeyReleased( const LJ_inputKey key );
LJ_bool LJ_inputKeyClicked( const LJ_inputKey key );

LJ_char* LJ_inputGetKeyName( const LJ_inputKey key );

// the state of the modifier keys e.g. shift, control, alt, etc.
LJ_inputKeyModifier LJ_inputGetKeyModifier( void );

///////////////////////////////////////////////////////////////////////////////////
//
// Mouse support
//
///////////////////////////////////////////////////////////////////////////////////

typedef enum LJ_inputMouseButtonEnum LJ_inputMouseButton;

void LJ_inputMouseGetPosition( LJ_int* const x, LJ_int* const y );
void LJ_inputMouseGetMovement( LJ_int* const deltaX, LJ_int* const deltaY );

LJ_bool LJ_inputMouseButtonPressed( const LJ_inputMouseButton button );
LJ_bool LJ_inputMouseButtonReleased( const LJ_inputMouseButton button );
LJ_bool LJ_inputMouseButtonClicked( const LJ_inputMouseButton button );

#endif // #ifndef LJ_INPUT_H

