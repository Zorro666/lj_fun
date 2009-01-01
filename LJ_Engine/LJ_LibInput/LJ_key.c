#include "LJ_key.h"

#include "LJ_internal_key.h"

#include <string.h>
#include <stdio.h>

#define LJ_NUM_KEYS (LJ_KEY_FINISH - LJ_KEY_START + 1)

typedef struct LJ_keyPrivateData
{
	char* keyNames[LJ_NUM_KEYS];

	LJ_inputKeyStateEnum keyStateData[2][LJ_NUM_KEYS];
	int keyModifierStatusData[2];

	LJ_inputKeyStateEnum* keyPrevState;
	LJ_inputKeyStateEnum* keyThisState;

	int* keyModifierStatusPrev;
	int* keyModifierStatusThis;
} LJ_keyPrivateData;

static LJ_keyPrivateData s_keyPrivateData;

void LJ_keyInit( void )
{
	s_keyPrivateData.keyNames[LJ_KEY_UNKNOWN] 				= "UNKNOWN";
	s_keyPrivateData.keyNames[LJ_KEY_BACKSPACE] 			= "BACKSPACE";
	s_keyPrivateData.keyNames[LJ_KEY_TAB] 					= "TAB";
	s_keyPrivateData.keyNames[LJ_KEY_CLEAR] 				= "CLEAR";
	s_keyPrivateData.keyNames[LJ_KEY_RETURN] 				= "RETURN";
	s_keyPrivateData.keyNames[LJ_KEY_PAUSE	] 				= "PAUSE";
	s_keyPrivateData.keyNames[LJ_KEY_ESCAPE] 				= "ESCAPE";
	s_keyPrivateData.keyNames[LJ_KEY_SPACE] 				= "SPACE";
	s_keyPrivateData.keyNames[LJ_KEY_EXCLAIM] 				= "!";
	s_keyPrivateData.keyNames[LJ_KEY_QUOTEDBL] 			= "\"";
	s_keyPrivateData.keyNames[LJ_KEY_HASH] 				= "#";
	s_keyPrivateData.keyNames[LJ_KEY_DOLLAR] 				= "$";
	s_keyPrivateData.keyNames[LJ_KEY_AMPERSAND] 			= "&";
	s_keyPrivateData.keyNames[LJ_KEY_QUOTE] 				= "'";
	s_keyPrivateData.keyNames[LJ_KEY_LEFTPAREN] 			= "(";
	s_keyPrivateData.keyNames[LJ_KEY_RIGHTPAREN] 			= ")";
	s_keyPrivateData.keyNames[LJ_KEY_ASTERISK] 			= "*";
	s_keyPrivateData.keyNames[LJ_KEY_PLUS] 				= "+";
	s_keyPrivateData.keyNames[LJ_KEY_COMMA] 				= ",";
	s_keyPrivateData.keyNames[LJ_KEY_MINUS] 				= "-";
	s_keyPrivateData.keyNames[LJ_KEY_DOT] 					= ".";
	s_keyPrivateData.keyNames[LJ_KEY_SLASH] 				= "/";
	s_keyPrivateData.keyNames[LJ_KEY_0] 					= "0";
	s_keyPrivateData.keyNames[LJ_KEY_1] 					= "1";
	s_keyPrivateData.keyNames[LJ_KEY_2] 					= "2";
	s_keyPrivateData.keyNames[LJ_KEY_3] 					= "3";
	s_keyPrivateData.keyNames[LJ_KEY_4] 					= "4";
	s_keyPrivateData.keyNames[LJ_KEY_5] 					= "5";
	s_keyPrivateData.keyNames[LJ_KEY_6] 					= "6";
	s_keyPrivateData.keyNames[LJ_KEY_7] 					= "7";
	s_keyPrivateData.keyNames[LJ_KEY_8] 					= "8";
	s_keyPrivateData.keyNames[LJ_KEY_9] 					= "9";
	s_keyPrivateData.keyNames[LJ_KEY_COLON] 				= ":";
	s_keyPrivateData.keyNames[LJ_KEY_SEMICOLON] 			= ";";
	s_keyPrivateData.keyNames[LJ_KEY_LESS] 				= "<";
	s_keyPrivateData.keyNames[LJ_KEY_EQUALS] 				= "=";
	s_keyPrivateData.keyNames[LJ_KEY_GREATER] 				= ">";
	s_keyPrivateData.keyNames[LJ_KEY_QUESTION] 			= "?";
	s_keyPrivateData.keyNames[LJ_KEY_AT] 					= "@";
	
	// Skip uppercase letters
	s_keyPrivateData.keyNames[LJ_KEY_LEFTBRACKET] 			= "[";
	s_keyPrivateData.keyNames[LJ_KEY_BACKSLASH] 			= "\\";
	s_keyPrivateData.keyNames[LJ_KEY_RIGHTBRACKET] 		= "]";
	s_keyPrivateData.keyNames[LJ_KEY_CARET] 				= "~";
	s_keyPrivateData.keyNames[LJ_KEY_UNDERSCORE] 			= "_";
	s_keyPrivateData.keyNames[LJ_KEY_BACKQUOTE] 			= "`";
	s_keyPrivateData.keyNames[LJ_KEY_a] 					= "A";
	s_keyPrivateData.keyNames[LJ_KEY_b] 					= "B";
	s_keyPrivateData.keyNames[LJ_KEY_c] 					= "C";
	s_keyPrivateData.keyNames[LJ_KEY_d] 					= "D";
	s_keyPrivateData.keyNames[LJ_KEY_e] 					= "E";
	s_keyPrivateData.keyNames[LJ_KEY_f] 					= "F";
	s_keyPrivateData.keyNames[LJ_KEY_g] 					= "G";
	s_keyPrivateData.keyNames[LJ_KEY_h] 					= "H";
	s_keyPrivateData.keyNames[LJ_KEY_i] 					= "I";
	s_keyPrivateData.keyNames[LJ_KEY_j] 					= "J";
	s_keyPrivateData.keyNames[LJ_KEY_k] 					= "K";
	s_keyPrivateData.keyNames[LJ_KEY_l] 					= "L";
	s_keyPrivateData.keyNames[LJ_KEY_m] 					= "M";
	s_keyPrivateData.keyNames[LJ_KEY_n] 					= "N";
	s_keyPrivateData.keyNames[LJ_KEY_o] 					= "O";
	s_keyPrivateData.keyNames[LJ_KEY_p] 					= "P";
	s_keyPrivateData.keyNames[LJ_KEY_q] 					= "Q";
	s_keyPrivateData.keyNames[LJ_KEY_r] 					= "R";
	s_keyPrivateData.keyNames[LJ_KEY_s] 					= "S";
	s_keyPrivateData.keyNames[LJ_KEY_t] 					= "T";
	s_keyPrivateData.keyNames[LJ_KEY_u] 					= "U";
	s_keyPrivateData.keyNames[LJ_KEY_v] 					= "V";
	s_keyPrivateData.keyNames[LJ_KEY_w] 					= "W";
	s_keyPrivateData.keyNames[LJ_KEY_x] 					= "X";
	s_keyPrivateData.keyNames[LJ_KEY_y] 					= "Y";
	s_keyPrivateData.keyNames[LJ_KEY_z] 					= "Z";
	s_keyPrivateData.keyNames[LJ_KEY_DELETE] 				= "DELETE";
	// End of ASCII mapped keys
	
	// Numeric keypad 
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD0] 				= "NUMPAD 0";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD1] 				= "NUMPAD 1";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD2] 				= "NUMPAD 2";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD3] 				= "NUMPAD 3";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD4] 				= "NUMPAD 4";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD5] 				= "NUMPAD 5";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD6] 				= "NUMPAD 6";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD7] 				= "NUMPAD 7";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD8] 				= "NUMPAD 8";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD9] 				= "NUMPAD 9";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD_DOT] 			= "NUMPAD DOT";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD_DIVIDE] 		= "NUMPAD DIVIDE";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD_MULTIPLY] 		= "NUMPAD TIMES";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD_MINUS] 		= "NUMPAD MINUS";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD_PLUS] 			= "NUMPAD PLUS";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD_ENTER] 		= "NUMPAD ENTER";
	s_keyPrivateData.keyNames[LJ_KEY_NUMPAD_EQUALS] 		= "NUMPAD EQUALS";
	
		// Arrows + Home/End pad 
	s_keyPrivateData.keyNames[LJ_KEY_UP] 					= "UP";
	s_keyPrivateData.keyNames[LJ_KEY_DOWN] 				= "DOWN";
	s_keyPrivateData.keyNames[LJ_KEY_RIGHT] 				= "RIGHT";
	s_keyPrivateData.keyNames[LJ_KEY_LEFT] 				= "LEFT";
	s_keyPrivateData.keyNames[LJ_KEY_INSERT] 				= "INSERT";
	s_keyPrivateData.keyNames[LJ_KEY_HOME] 				= "HOME";
	s_keyPrivateData.keyNames[LJ_KEY_END] 					= "END";
	s_keyPrivateData.keyNames[LJ_KEY_PAGEUP] 				= "PAGE UP";
	s_keyPrivateData.keyNames[LJ_KEY_PAGEDOWN] 			= "PAGE DOWN";
	
	// Function keys 
	s_keyPrivateData.keyNames[LJ_KEY_F1] 					= "F1";
	s_keyPrivateData.keyNames[LJ_KEY_F2] 					= "F2";
	s_keyPrivateData.keyNames[LJ_KEY_F3] 					= "F3";
	s_keyPrivateData.keyNames[LJ_KEY_F4] 					= "F4";
	s_keyPrivateData.keyNames[LJ_KEY_F5] 					= "F5";
	s_keyPrivateData.keyNames[LJ_KEY_F6] 					= "F6";
	s_keyPrivateData.keyNames[LJ_KEY_F7] 					= "F7";
	s_keyPrivateData.keyNames[LJ_KEY_F8] 					= "F8";
	s_keyPrivateData.keyNames[LJ_KEY_F9] 					= "F9";
	s_keyPrivateData.keyNames[LJ_KEY_F10] 					= "F10";
	s_keyPrivateData.keyNames[LJ_KEY_F11] 					= "F11";
	s_keyPrivateData.keyNames[LJ_KEY_F12] 					= "F12";
	s_keyPrivateData.keyNames[LJ_KEY_F13] 					= "F13";
	s_keyPrivateData.keyNames[LJ_KEY_F14] 					= "F14";
	s_keyPrivateData.keyNames[LJ_KEY_F15] 					= "F15";
	
	// modifier keys 
	s_keyPrivateData.keyNames[LJ_KEY_NUMLOCK] 				= "NUM LOCK";
	s_keyPrivateData.keyNames[LJ_KEY_CAPSLOCK] 			= "CAPS LOCK";
	s_keyPrivateData.keyNames[LJ_KEY_SCROLLOCK] 			= "SCROLL LOCK";
	s_keyPrivateData.keyNames[LJ_KEY_RSHIFT] 				= "RSHIFT";
	s_keyPrivateData.keyNames[LJ_KEY_LSHIFT] 				= "LSHIFT";
	s_keyPrivateData.keyNames[LJ_KEY_RCTRL] 				= "RCTRL";
	s_keyPrivateData.keyNames[LJ_KEY_LCTRL] 				= "LCTRL";
	s_keyPrivateData.keyNames[LJ_KEY_RALT] 				= "RALT";
	s_keyPrivateData.keyNames[LJ_KEY_LALT] 				= "LALT";
	s_keyPrivateData.keyNames[LJ_KEY_RMETA] 				= "RMETA";
	s_keyPrivateData.keyNames[LJ_KEY_LMETA] 				= "LMETA";
	s_keyPrivateData.keyNames[LJ_KEY_LWINDOWS] 			= "LWINDOWS";
	s_keyPrivateData.keyNames[LJ_KEY_RWINDOWS] 			= "RWINDOWS";
	s_keyPrivateData.keyNames[LJ_KEY_MODE] 				= "MODE";
	
	// Miscellaneous function keys 
	s_keyPrivateData.keyNames[LJ_KEY_HELP] 				= "HELP";
	s_keyPrivateData.keyNames[LJ_KEY_PRINT] 				= "PRINT";
	s_keyPrivateData.keyNames[LJ_KEY_SYSREQ] 				= "SYSREQ";
	s_keyPrivateData.keyNames[LJ_KEY_BREAK] 				= "BREAK";
	s_keyPrivateData.keyNames[LJ_KEY_MENU] 				= "MENU";

	s_keyPrivateData.keyNames[LJ_KEY_APPACTIVE] 			= "APPACTIVE";
	s_keyPrivateData.keyNames[LJ_KEY_QUIT] 				= "QUIT";

	LJ_keyReset();
}

void LJ_keyReset( void )
{
	int i;

	s_keyPrivateData.keyPrevState = &s_keyPrivateData.keyStateData[0][0];
	s_keyPrivateData.keyThisState = &s_keyPrivateData.keyStateData[1][0];

	s_keyPrivateData.keyModifierStatusPrev = &s_keyPrivateData.keyModifierStatusData[0];
	s_keyPrivateData.keyModifierStatusThis = &s_keyPrivateData.keyModifierStatusData[1];

	// Clear out the states
	for ( i = 0; i < LJ_NUM_KEYS; i++ )
	{
		s_keyPrivateData.keyThisState[i] = LJ_KEY_STATE_UNKNOWN;
		s_keyPrivateData.keyPrevState[i] = LJ_KEY_STATE_UNKNOWN;
	}

	*s_keyPrivateData.keyModifierStatusPrev = LJ_KEY_MOD_NONE;
	*s_keyPrivateData.keyModifierStatusThis = LJ_KEY_MOD_NONE;
}

void LJ_keyTick( void )
{
	int i;

	// Toggle the double buffer data 
	LJ_inputKeyStateEnum* const temp = s_keyPrivateData.keyPrevState;
	s_keyPrivateData.keyPrevState = s_keyPrivateData.keyThisState;
	s_keyPrivateData.keyThisState = temp;

	// Copy last frame state to this state
	for ( i = 0; i < LJ_NUM_KEYS; i++ )
	{
		s_keyPrivateData.keyThisState[i] = s_keyPrivateData.keyPrevState[i];
	}
}

void LJ_keyShutdown( void )
{
}

char* LJ_keyGetKeyName( const LJ_inputKeyEnum key )
{
	if ( key >= LJ_KEY_FINISH )
	{
		return "UNKNOWN";
	}
	return s_keyPrivateData.keyNames[key];
}

void LJ_keyUp( const LJ_inputKeyEnum key, const int modifier )
{
	// Copy the this state to the prev state because can get up & down events in the same update loop
	s_keyPrivateData.keyPrevState[key] = s_keyPrivateData.keyThisState[key];
	s_keyPrivateData.keyThisState[key] = LJ_KEY_STATE_RELEASED;
	*s_keyPrivateData.keyModifierStatusThis = modifier;

	// TODO: Include modifiers up and down

	printf( "Key Up %d '%s'\n", key, LJ_keyGetKeyName( key ) );
}
 
void LJ_keyDown( const LJ_inputKeyEnum key, const int modifier )
{
	// Copy the this state to the prev state because can get up & down events in the same update loop
	s_keyPrivateData.keyPrevState[key] = s_keyPrivateData.keyThisState[key];
	s_keyPrivateData.keyThisState[key] = LJ_KEY_STATE_PRESSED;
	*s_keyPrivateData.keyModifierStatusThis = modifier;

	// TODO: Include modifiers up and down

	printf( "Key Down %d '%s'\n", key, LJ_keyGetKeyName( key ) );
}
 
LJ_inputKeyStateEnum LJ_keyGetKeyThisState( const LJ_inputKeyEnum key )
{
	return s_keyPrivateData.keyThisState[key];
}

LJ_inputKeyStateEnum LJ_keyGetKeyPrevState( const LJ_inputKeyEnum key )
{
	return s_keyPrivateData.keyPrevState[key];
}

LJ_inputKeyModifierEnum LJ_keyGetKeyThisModifier( void )
{
	return *s_keyPrivateData.keyModifierStatusThis;
}

LJ_inputKeyModifierEnum LJ_keyGetKeyPrevModifier( void )
{
	return *s_keyPrivateData.keyModifierStatusPrev;
}

