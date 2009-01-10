#include "LJ.h"

#include "LJ_LibCommon/LJ_LibCommon.h"

#include "LJ_mouse.h"

#include "LJ_internal_mouse.h"

#define LJ_NUM_MOUSE_BUTTONS (LJ_MOUSE_BUTTON_FINISH - LJ_MOUSE_BUTTON_START + 1)

typedef struct LJ_mousePrivateData
{
	LJ_char* mouseButtonNames[LJ_NUM_MOUSE_BUTTONS];

	LJ_inputMouseButtonStateEnum mouseButtonStateData[2][LJ_NUM_MOUSE_BUTTONS];

	LJ_inputMouseButtonStateEnum* mouseButtonPrevState;
	LJ_inputMouseButtonStateEnum* mouseButtonThisState;

	LJ_int mousePositionX;
	LJ_int mousePositionY;
	LJ_int mouseDeltaX;
	LJ_int mouseDeltaY;
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
	LJ_int i;

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
	LJ_int i;

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

LJ_char* LJ_mouseGetButtonName( const LJ_inputMouseButtonEnum button )
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

	LJ_outputPrintRelease( ( "Button Up %d '%s'\n", button, LJ_mouseGetButtonName( button ) ) );
}

void LJ_mouseButtonDown( const LJ_inputMouseButtonEnum button )
{
	// Copy the this state to the prev state because can get up & down events in the same update loop
	s_mousePrivateData.mouseButtonPrevState[button] = s_mousePrivateData.mouseButtonThisState[button];
	s_mousePrivateData.mouseButtonThisState[button] = LJ_MOUSE_BUTTON_STATE_PRESSED;

	LJ_outputPrintRelease( ( "Button Down %d '%s'\n", button, LJ_mouseGetButtonName( button ) ) );
}

void LJ_mouseSetPosition( const LJ_int x, const LJ_int y, const LJ_int xDelta, const LJ_int yDelta )
{
	s_mousePrivateData.mousePositionX = x;
	s_mousePrivateData.mousePositionY = y;
	s_mousePrivateData.mouseDeltaX = xDelta;
	s_mousePrivateData.mouseDeltaY = yDelta;

	LJ_outputPrintRelease( ( "Pos %d %d\n", x, y ) );
}

void LJ_mouseGetPosition( LJ_int* const x, LJ_int* const y )
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

void LJ_mouseGetMovement( LJ_int* const deltaX, LJ_int* const deltaY )
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

