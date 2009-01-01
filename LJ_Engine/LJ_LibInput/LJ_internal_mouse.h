#ifndef LJ_INTERNAL_MOUSE_HH
#define LJ_INTERNAL_MOUSE_HH

///////////////////////////////////////////////////////////////////////////////////
//
// Standard module functions
//
///////////////////////////////////////////////////////////////////////////////////

void LJ_mouseInit( void );
void LJ_mouseReset( void );
void LJ_mouseTick( void );
void LJ_mouseShutdown( void );

char* const LJ_mouseGetButtonName( const LJ_inputMouseButtonEnum button );

LJ_inputMouseButtonStateEnum LJ_mouseGetButtonPrevState( const LJ_inputMouseButtonEnum button );
LJ_inputMouseButtonStateEnum LJ_mouseGetButtonThisState( const LJ_inputMouseButtonEnum button );

void LJ_mouseGetPosition( int* const x, int* const y );
void LJ_mouseGetMovement( int* const deltaX, int* const deltaY );

void LJ_mouseButtonUp( const LJ_inputMouseButtonEnum button );
void LJ_mouseButtonDown( const LJ_inputMouseButtonEnum button );
void LJ_mouseSetPosition( const int x, const int y, const int xDelta, const int yDelta );

#endif // #ifndef LJ_INTERNAL_MOUSE_HH

