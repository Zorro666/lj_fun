#include "LJ_mouse.h"

#include "LJ_internal_mouse.h"

#include <stdio.h>

#define LJ_NUM_MOUSE_BUTTONS (LJ_MOUSE_BUTTON_FINISH - LJ_MOUSE_BUTTON_START + 1)

typedef struct LJ_mousePrivateData
{
	char* mouseButtonNames[LJ_NUM_MOUSE_BUTTONS];

	LJ_inputMouseButtonStateEnum mouseButtonStateData[2][LJ_NUM_MOUSE_BUTTONS];

	LJ_inputMouseButtonStateEnum* mouseButtonPrevState;
	LJ_inputMouseButtonStateEnum* mouseButtonThisState;

	int mousePositionX;
	int mousePositionY;
	int mouseDeltaX;
	int mouseDeltaY;
} LJ_mousePrivateData;

static LJ_mousePrivateData s_mousePrivateData;

void LJ_mouseInit( void )
{
	s_mousePrivateData.mouseButtonNames[LJ_MOUSE_BUTTON_UNKNOWN] 		= "UNKNOWN";
	s_mousePrivateData.mouseButtonNames[LJ_MOUSE_BUTTON_LEFT]			= "LEFT";
	s_mousePrivateData.mouseButtonNames[LJ_MOUSE_BUTTON_MIDDLE]			= "MIDDLE";
	s_mousePrivateData.mouseButtonNames[LJ_MOUSE_BUTTON_RIGHT]			= "RIGHT";
	s_mousePrivateData.mouseButtonNames[LJ_MOUSE_BUTTON_WHEELUP]		= "WHEELUP";
	s_mousePrivateData.mouseButtonNames[LJ_MOUSE_BUTTON_WHEELDOWN]		= "WHEELDOWN";

	LJ_mouseReset();
}

void LJ_mouseReset( void )
{
	int i;

	s_mousePrivateData.mouseButtonPrevState = &s_mousePrivateData.mouseButtonStateData[0][0];
	s_mousePrivateData.mouseButtonThisState = &s_mousePrivateData.mouseButtonStateData[1][0];

	// Clear out the states
	for ( i = 0; i < LJ_NUM_MOUSE_BUTTONS; i++ )
	{
		s_mousePrivateData.mouseButtonThisState[i] = LJ_MOUSE_BUTTON_STATE_UNKNOWN;
		s_mousePrivateData.mouseButtonPrevState[i] = LJ_MOUSE_BUTTON_STATE_UNKNOWN;
	}

	s_mousePrivateData.mousePositionX = -1;
	s_mousePrivateData.mousePositionY = -1;
	s_mousePrivateData.mouseDeltaX = 0;
	s_mousePrivateData.mouseDeltaY = 0;
}

void LJ_mouseTick( void )
{
	int i;

	// Toggle the double buffer data 
	LJ_inputMouseButtonStateEnum* const temp = s_mousePrivateData.mouseButtonPrevState;
	s_mousePrivateData.mouseButtonPrevState = s_mousePrivateData.mouseButtonThisState;
	s_mousePrivateData.mouseButtonThisState = temp;

	// Copy last frame state to this state
	for ( i = 0; i < LJ_NUM_MOUSE_BUTTONS; i++ )
	{
		s_mousePrivateData.mouseButtonThisState[i] = s_mousePrivateData.mouseButtonPrevState[i];
	}
}

void LJ_mouseShutdown( void )
{
}

char* LJ_mouseGetButtonName( const LJ_inputMouseButtonEnum button )
{
	return s_mousePrivateData.mouseButtonNames[button];
}

LJ_inputMouseButtonStateEnum LJ_mouseGetButtonPrevState( const LJ_inputMouseButtonEnum button )
{
	return s_mousePrivateData.mouseButtonPrevState[button];
}

LJ_inputMouseButtonStateEnum LJ_mouseGetButtonThisState( const LJ_inputMouseButtonEnum button )
{
	return s_mousePrivateData.mouseButtonThisState[button];
}

void LJ_mouseButtonUp( const LJ_inputMouseButtonEnum button )
{
	// Copy the this state to the prev state because can get up & down events in the same update loop
	s_mousePrivateData.mouseButtonPrevState[button] = s_mousePrivateData.mouseButtonThisState[button];
	s_mousePrivateData.mouseButtonThisState[button] = LJ_MOUSE_BUTTON_STATE_RELEASED;

	printf( "Button Up %d '%s'\n", button, LJ_mouseGetButtonName( button ) );
}

void LJ_mouseButtonDown( const LJ_inputMouseButtonEnum button )
{
	// Copy the this state to the prev state because can get up & down events in the same update loop
	s_mousePrivateData.mouseButtonPrevState[button] = s_mousePrivateData.mouseButtonThisState[button];
	s_mousePrivateData.mouseButtonThisState[button] = LJ_MOUSE_BUTTON_STATE_PRESSED;

	printf( "Button Down %d '%s'\n", button, LJ_mouseGetButtonName( button ) );
}

void LJ_mouseSetPosition( const int x, const int y, const int xDelta, const int yDelta )
{
	s_mousePrivateData.mousePositionX = x;
	s_mousePrivateData.mousePositionY = y;
	s_mousePrivateData.mouseDeltaX = xDelta;
	s_mousePrivateData.mouseDeltaY = yDelta;

	printf( "Pos %d %d\n", x, y );
}

void LJ_mouseGetPosition( int* const x, int* const y )
{
	if ( x )
	{
		*x = s_mousePrivateData.mousePositionX;
	}
	if ( y )
	{
		*y = s_mousePrivateData.mousePositionY;
	}
}

void LJ_mouseGetMovement( int* const deltaX, int* const deltaY )
{
	if ( deltaX )
	{
		*deltaX = s_mousePrivateData.mouseDeltaX;
	}
	if ( deltaY )
	{
		*deltaY = s_mousePrivateData.mouseDeltaY;
	}
}

