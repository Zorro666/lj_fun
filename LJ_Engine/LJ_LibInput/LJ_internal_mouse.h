#ifndef LJ_INTERNAL_MOUSE_HH
#define LJ_INTERNAL_MOUSE_HH

#include "LJ_LibCommon/LJ_LibCommon.h"

///////////////////////////////////////////////////////////////////////////////////
//
// Standard module functions
//
///////////////////////////////////////////////////////////////////////////////////

void LJ_mouseInit( void );
void LJ_mouseReset( void );
void LJ_mouseTick( void );
void LJ_mouseShutdown( void );

LJ_char* LJ_mouseGetButtonName( const LJ_inputMouseButtonEnum button );

LJ_inputMouseButtonStateEnum LJ_mouseGetButtonPrevState( const LJ_inputMouseButtonEnum button );
LJ_inputMouseButtonStateEnum LJ_mouseGetButtonThisState( const LJ_inputMouseButtonEnum button );

void LJ_mouseGetPosition( LJ_int* const x, LJ_int* const y );
void LJ_mouseGetMovement( LJ_int* const deltaX, LJ_int* const deltaY );

void LJ_mouseButtonUp( const LJ_inputMouseButtonEnum button );
void LJ_mouseButtonDown( const LJ_inputMouseButtonEnum button );
void LJ_mouseSetPosition( const LJ_int x, const LJ_int y, const LJ_int xDelta, const LJ_int yDelta );

#endif // #ifndef LJ_INTERNAL_MOUSE_HH

