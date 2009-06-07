#ifndef LJ_INTERNAL_KEY_H
#define LJ_INTERNAL_KEY_H

#include "LJ_LibCommon/LJ_LibCommon.h"

///////////////////////////////////////////////////////////////////////////////////
//
// Standard module functions
//
///////////////////////////////////////////////////////////////////////////////////

void LJ_keyInit( void );
void LJ_keyReset( void );
void LJ_keyTick( void );
void LJ_keyShutdown( void );

LJ_char* LJ_keyGetKeyName( const LJ_inputKeyEnum key );
LJ_inputKeyStateEnum LJ_keyGetKeyPrevState( const LJ_inputKeyEnum key );
LJ_inputKeyStateEnum LJ_keyGetKeyThisState( const LJ_inputKeyEnum key );

LJ_inputKeyModifierEnum LJ_keyGetKeyThisModifier( void );
LJ_inputKeyModifierEnum LJ_keyGetKeyPrevModifier( void );

void LJ_keyUp( const LJ_inputKeyEnum key, const LJ_int modifier );
void LJ_keyDown( const LJ_inputKeyEnum key, const LJ_int modifier );

#endif // #ifndef LJ_INTERNAL_KEY_H
