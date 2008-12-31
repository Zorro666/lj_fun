#include "LJ_input.h"
#include "LJ_key.h"
#include "LJ_internal_key.h"

#include <SDL/SDL.h>

void LJ_MouseMoved( const int x, const int y, const int relX, const int relY );
void LJ_MouseButtonUp( const int button, const int x, const int y, const int relX, const int relY );
void LJ_MouseButtonDown( const int button, const int x, const int y, const int relX, const int relY );

void LJ_inputInit( void )
{
	LJ_keyInit();
}

void LJ_inputReset( void )
{
	LJ_keyReset();
}

void LJ_inputShutdown( void )
{
	LJ_keyShutdown();
}

void LJ_inputTick( void )
{
	SDL_Event event;

	// Need to swap the key buffers about to detect key clicking
	LJ_keyTick();

	// Handle the events we are interested in
	if ( SDL_PollEvent( &event ) ) 
	{
		const int keyType = event.type;
		switch ( keyType ) 
		{
			case SDL_KEYDOWN:
			{
				const SDL_KeyboardEvent* const keyEvent = &event.key;
				const LJ_inputKeyEnum key = keyEvent->keysym.sym;
				const int modifier = keyEvent->keysym.mod;
				LJ_keyDown( key, modifier );
				break;
			}
 
			case SDL_KEYUP:
			{
				const SDL_KeyboardEvent* const keyEvent = &event.key;
				const LJ_inputKeyEnum key = keyEvent->keysym.sym;
				const int modifier = keyEvent->keysym.mod;
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
				const int x = event.motion.x;
				const int y = event.motion.y;
				const int xrel = event.motion.xrel;
				const int yrel = event.motion.yrel;
				LJ_MouseMoved( x, y, xrel, yrel );
				break;
			}
 
			case SDL_MOUSEBUTTONUP:
			{
				const int button = event.button.button;
				const int x = event.motion.x;
				const int y = event.motion.y;
				const int relX = event.motion.xrel;
				const int relY = event.motion.yrel;
				LJ_MouseButtonUp( button, x, y, relX, relY );
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				const int button = event.button.button;
				const int x = event.motion.x;
				const int y = event.motion.y;
				const int relX = event.motion.xrel;
				const int relY = event.motion.yrel;
				LJ_MouseButtonDown( button, x, y, relX, relY );
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

void LJ_MouseMoved( const int x, const int y, const int relX, const int relY )
{
}
 
void LJ_MouseButtonUp( const int button, const int x, const int y, const int relX, const int relY )
{
}

void LJ_MouseButtonDown( const int button, const int x, const int y, const int relX, const int relY )
{
}


int LJ_inputKeyPressed( const LJ_inputKey key )
{
	const LJ_inputKeyStateEnum value = LJ_keyGetKeyThisState( key );
	return ( value == LJ_KEY_STATE_PRESSED );
}

int LJ_inputKeyReleased( const LJ_inputKey key )
{
	const LJ_inputKeyStateEnum value = LJ_keyGetKeyThisState( key );
	return ( value == LJ_KEY_STATE_RELEASED );
}

int LJ_inputKeyClicked( const LJ_inputKey key )
{
	const LJ_inputKeyStateEnum prevValue = LJ_keyGetKeyPrevState( key );
	const LJ_inputKeyStateEnum thisValue = LJ_keyGetKeyThisState( key );

	if ( ( prevValue == LJ_KEY_STATE_PRESSED ) && ( thisValue == LJ_KEY_STATE_RELEASED ) )
	{
		return 1;
	}

	return 0;
}

char* const LJ_inputGetKeyName( const LJ_inputKey key )
{
	return LJ_keyGetKeyName( key );
}

LJ_inputKeyModifier LJ_inputGetKeyModifier( void )
{
	return LJ_keyGetKeyThisModifier();
}

