#include "LJ.h"

#include "LJ_LibCommon/LJ_LibCommon.h"

#include "LJ_input.h"
#include "LJ_key.h"
#include "LJ_mouse.h"
#include "LJ_internal_key.h"
#include "LJ_internal_mouse.h"

#include <SDL.h>

///////////////////////////////////////////////////////////////////////////////////
//
// Standard module functions
//
///////////////////////////////////////////////////////////////////////////////////

void LJ_inputInit( void )
{
	LJ_keyInit();
	LJ_mouseInit();
}

void LJ_inputReset( void )
{
	LJ_keyReset();
	LJ_mouseReset();
}

void LJ_inputShutdown( void )
{
	LJ_keyShutdown();
	LJ_mouseShutdown();
}

void LJ_inputTick( void )
{
	SDL_Event event;

	// Tick the subsystems
	LJ_keyTick();
	LJ_mouseTick();

	// Drain the event queue of keyboard, mouse events
	while ( SDL_PollEvent( &event ) ) 
	{
		const LJ_int keyType = event.type;
		switch ( keyType ) 
		{
			case SDL_KEYDOWN:
			{
				const SDL_KeyboardEvent* const keyEvent = &event.key;
				const LJ_inputKeyEnum key = keyEvent->keysym.sym;
				const LJ_int modifier = keyEvent->keysym.mod;
				LJ_keyDown( key, modifier );
				break;
			}
 
			case SDL_KEYUP:
			{
				const SDL_KeyboardEvent* const keyEvent = &event.key;
				const LJ_inputKeyEnum key = keyEvent->keysym.sym;
				const LJ_int modifier = keyEvent->keysym.mod;
				LJ_keyUp( key, modifier );
				break;
			}
 
			case SDL_QUIT:
			{
				LJ_keyDown( SDL_QUIT, 0 );
				break;
			}
 
			case SDL_MOUSEMOTION:
			{
				const LJ_int x = event.motion.x;
				const LJ_int y = event.motion.y;
				const LJ_int xDelta = event.motion.xrel;
				const LJ_int yDelta = event.motion.yrel;
				LJ_mouseSetPosition( x, y, xDelta, yDelta );
				break;
			}
 
			case SDL_MOUSEBUTTONUP:
			{
				const LJ_int button = event.button.button;
				//const LJ_int x = event.motion.x;
				//const LJ_int y = event.motion.y;
				//const LJ_int xDelta = event.motion.xrel;
				//const LJ_int yDelta = event.motion.yrel;
				LJ_mouseButtonUp( button );
				//LJ_mouseSetPosition( x, y, xDelta, yDelta );
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				const LJ_int button = event.button.button;
				//const LJ_int x = event.motion.x;
				//const LJ_int y = event.motion.y;
				//const LJ_int xDelta = event.motion.xrel;
				//const LJ_int yDelta = event.motion.yrel;
				LJ_mouseButtonDown( button );
				//LJ_mouseSetPosition( x, y, xDelta, yDelta );
				break;
			}

			case SDL_ACTIVEEVENT:
			{
				if ( event.active.state & SDL_APPACTIVE ) 
				{
					if ( event.active.gain ) 
					{
						LJ_keyDown( SDL_APPACTIVE, 0 );
					} 
					else 
					{
						LJ_keyUp( SDL_APPACTIVE, 0 );
					}
				}
				break;
			}
		}
	} 
}

///////////////////////////////////////////////////////////////////////////////////
//
// Keyboard support
//
///////////////////////////////////////////////////////////////////////////////////

LJ_bool LJ_inputKeyPressed( const LJ_inputKey key )
{
	const LJ_inputKeyStateEnum value = LJ_keyGetKeyThisState( key );
	return ( value == LJ_KEY_STATE_PRESSED );
}

LJ_bool LJ_inputKeyReleased( const LJ_inputKey key )
{
	const LJ_inputKeyStateEnum value = LJ_keyGetKeyThisState( key );
	return ( value == LJ_KEY_STATE_RELEASED );
}

LJ_bool LJ_inputKeyClicked( const LJ_inputKey key )
{
	const LJ_inputKeyStateEnum prevValue = LJ_keyGetKeyPrevState( key );
	const LJ_inputKeyStateEnum thisValue = LJ_keyGetKeyThisState( key );

	if ( ( prevValue == LJ_KEY_STATE_PRESSED ) && ( thisValue == LJ_KEY_STATE_RELEASED ) )
	{
		return 1;
	}

	return 0;
}

LJ_char* LJ_inputGetKeyName( const LJ_inputKey key )
{
	return LJ_keyGetKeyName( key );
}

LJ_inputKeyModifier LJ_inputGetKeyModifier( void )
{
	return LJ_keyGetKeyThisModifier();
}

///////////////////////////////////////////////////////////////////////////////////
//
// Mouse support
//
///////////////////////////////////////////////////////////////////////////////////

void LJ_inputMouseGetPosition( LJ_int* const x, LJ_int* const y )
{
	LJ_mouseGetPosition( x, y );
}

void LJ_inputMouseGetMovement( LJ_int* const deltaX, LJ_int* const deltaY )
{
	LJ_mouseGetMovement( deltaX, deltaY );
}

LJ_bool LJ_inputMouseButtonPressed( const LJ_inputMouseButton button )
{
	const LJ_inputMouseButtonStateEnum value = LJ_mouseGetButtonThisState( button );
	return ( ( value == LJ_MOUSE_BUTTON_STATE_PRESSED ) ? LJ_TRUE : LJ_FALSE );
}

LJ_bool LJ_inputMouseButtonReleased( const LJ_inputMouseButton button )
{
	const LJ_inputMouseButtonStateEnum value = LJ_mouseGetButtonThisState( button );
	return ( ( value == LJ_MOUSE_BUTTON_STATE_RELEASED ) ? LJ_TRUE : LJ_FALSE );
}

LJ_bool LJ_inputMouseButtonClicked( const LJ_inputMouseButton button )
{
	const LJ_inputMouseButtonStateEnum prevValue = LJ_mouseGetButtonPrevState( button );
	const LJ_inputMouseButtonStateEnum thisValue = LJ_mouseGetButtonThisState( button );

	if ( ( prevValue == LJ_MOUSE_BUTTON_STATE_PRESSED ) && ( thisValue == LJ_MOUSE_BUTTON_STATE_RELEASED ) )
	{
		return LJ_TRUE;
	}

	return LJ_FALSE;
}

