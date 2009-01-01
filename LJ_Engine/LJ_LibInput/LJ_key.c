#include "LJ_key.h"

#include "LJ_internal_key.h"

#include <string.h>
#include <stdio.h>

#define LJ_NUM_KEYS (LJ_KEY_FINISH - LJ_KEY_START + 1)

char* s_keyNames[LJ_NUM_KEYS];

LJ_inputKeyStateEnum s_keyStateData[2][LJ_NUM_KEYS];
int s_keyModifierStatusData[2] = { LJ_KEY_MOD_NONE, LJ_KEY_MOD_NONE };

LJ_inputKeyStateEnum* s_keyPrevState;
LJ_inputKeyStateEnum* s_keyThisState;

int* s_keyModifierStatusPrev = NULL;
int* s_keyModifierStatusThis = NULL;

void LJ_keyInit( void )
{
	s_keyNames[LJ_KEY_UNKNOWN] = 			"UNKNOWN";
	s_keyNames[LJ_KEY_BACKSPACE] = 			"BACKSPACE";
	s_keyNames[LJ_KEY_TAB] = 				"TAB";
	s_keyNames[LJ_KEY_CLEAR] = 				"CLEAR";
	s_keyNames[LJ_KEY_RETURN] = 			"RETURN";
	s_keyNames[LJ_KEY_PAUSE	] = 			"PAUSE";
	s_keyNames[LJ_KEY_ESCAPE] = 			"ESCAPE";
	s_keyNames[LJ_KEY_SPACE] = 				"SPACE";
	s_keyNames[LJ_KEY_EXCLAIM] = 			"!";
	s_keyNames[LJ_KEY_QUOTEDBL] = 			"\"";
	s_keyNames[LJ_KEY_HASH] = 				"#";
	s_keyNames[LJ_KEY_DOLLAR] = 			"$";
	s_keyNames[LJ_KEY_AMPERSAND] = 			"&";
	s_keyNames[LJ_KEY_QUOTE] = 				"'";
	s_keyNames[LJ_KEY_LEFTPAREN] = 			"(";
	s_keyNames[LJ_KEY_RIGHTPAREN] = 		")";
	s_keyNames[LJ_KEY_ASTERISK] = 			"*";
	s_keyNames[LJ_KEY_PLUS] = 				"+";
	s_keyNames[LJ_KEY_COMMA] = 				",";
	s_keyNames[LJ_KEY_MINUS] = 				"-";
	s_keyNames[LJ_KEY_DOT] = 				".";
	s_keyNames[LJ_KEY_SLASH] = 				"/";
	s_keyNames[LJ_KEY_0] = 					"0";
	s_keyNames[LJ_KEY_1] = 					"1";
	s_keyNames[LJ_KEY_2] = 					"2";
	s_keyNames[LJ_KEY_3] = 					"3";
	s_keyNames[LJ_KEY_4] = 					"4";
	s_keyNames[LJ_KEY_5] = 					"5";
	s_keyNames[LJ_KEY_6] = 					"6";
	s_keyNames[LJ_KEY_7] = 					"7";
	s_keyNames[LJ_KEY_8] = 					"8";
	s_keyNames[LJ_KEY_9] = 					"9";
	s_keyNames[LJ_KEY_COLON] = 				":";
	s_keyNames[LJ_KEY_SEMICOLON] = 			";";
	s_keyNames[LJ_KEY_LESS] = 				"<";
	s_keyNames[LJ_KEY_EQUALS] = 			"=";
	s_keyNames[LJ_KEY_GREATER] = 			">";
	s_keyNames[LJ_KEY_QUESTION] = 			"?";
	s_keyNames[LJ_KEY_AT] = 				"@";
	
		//   Skip uppercase letters
	s_keyNames[LJ_KEY_LEFTBRACKET] = 		"[";
	s_keyNames[LJ_KEY_BACKSLASH] = 			"\\";
	s_keyNames[LJ_KEY_RIGHTBRACKET] = 		"]";
	s_keyNames[LJ_KEY_CARET] = 				"~";
	s_keyNames[LJ_KEY_UNDERSCORE] = 		"_";
	s_keyNames[LJ_KEY_BACKQUOTE] = 			"`";
	s_keyNames[LJ_KEY_a] = 					"A";
	s_keyNames[LJ_KEY_b] = 					"B";
	s_keyNames[LJ_KEY_c] = 					"C";
	s_keyNames[LJ_KEY_d] = 					"D";
	s_keyNames[LJ_KEY_e] = 					"E";
	s_keyNames[LJ_KEY_f] = 					"F";
	s_keyNames[LJ_KEY_g] = 					"G";
	s_keyNames[LJ_KEY_h] = 					"H";
	s_keyNames[LJ_KEY_i] = 					"I";
	s_keyNames[LJ_KEY_j] = 					"J";
	s_keyNames[LJ_KEY_k] = 					"K";
	s_keyNames[LJ_KEY_l] = 					"L";
	s_keyNames[LJ_KEY_m] = 					"M";
	s_keyNames[LJ_KEY_n] = 					"N";
	s_keyNames[LJ_KEY_o] = 					"O";
	s_keyNames[LJ_KEY_p] = 					"P";
	s_keyNames[LJ_KEY_q] = 					"Q";
	s_keyNames[LJ_KEY_r] = 					"R";
	s_keyNames[LJ_KEY_s] = 					"S";
	s_keyNames[LJ_KEY_t] = 					"T";
	s_keyNames[LJ_KEY_u] = 					"U";
	s_keyNames[LJ_KEY_v] = 					"V";
	s_keyNames[LJ_KEY_w] = 					"W";
	s_keyNames[LJ_KEY_x] = 					"X";
	s_keyNames[LJ_KEY_y] = 					"Y";
	s_keyNames[LJ_KEY_z] = 					"Z";
	s_keyNames[LJ_KEY_DELETE] = 			"DELETE";
		// End of ASCII mapped keys
	
		// Numeric keypad 
	s_keyNames[LJ_KEY_NUMPAD0] = 			"NUMPAD 0";
	s_keyNames[LJ_KEY_NUMPAD1] = 			"NUMPAD 1";
	s_keyNames[LJ_KEY_NUMPAD2] = 			"NUMPAD 2";
	s_keyNames[LJ_KEY_NUMPAD3] = 			"NUMPAD 3";
	s_keyNames[LJ_KEY_NUMPAD4] = 			"NUMPAD 4";
	s_keyNames[LJ_KEY_NUMPAD5] = 			"NUMPAD 5";
	s_keyNames[LJ_KEY_NUMPAD6] = 			"NUMPAD 6";
	s_keyNames[LJ_KEY_NUMPAD7] = 			"NUMPAD 7";
	s_keyNames[LJ_KEY_NUMPAD8] = 			"NUMPAD 8";
	s_keyNames[LJ_KEY_NUMPAD9] = 			"NUMPAD 9";
	s_keyNames[LJ_KEY_NUMPAD_DOT] = 		"NUMPAD DOT";
	s_keyNames[LJ_KEY_NUMPAD_DIVIDE] = 		"NUMPAD DIVIDE";
	s_keyNames[LJ_KEY_NUMPAD_MULTIPLY] = 	"NUMPAD TIMES";
	s_keyNames[LJ_KEY_NUMPAD_MINUS] = 		"NUMPAD MINUS";
	s_keyNames[LJ_KEY_NUMPAD_PLUS] = 		"NUMPAD PLUS";
	s_keyNames[LJ_KEY_NUMPAD_ENTER] = 		"NUMPAD ENTER";
	s_keyNames[LJ_KEY_NUMPAD_EQUALS] = 		"NUMPAD EQUALS";
	
		// Arrows + Home/End pad 
	s_keyNames[LJ_KEY_UP] = 				"UP";
	s_keyNames[LJ_KEY_DOWN] = 				"DOWN";
	s_keyNames[LJ_KEY_RIGHT] = 				"RIGHT";
	s_keyNames[LJ_KEY_LEFT] = 				"LEFT";
	s_keyNames[LJ_KEY_INSERT] = 			"INSERT";
	s_keyNames[LJ_KEY_HOME] = 				"HOME";
	s_keyNames[LJ_KEY_END] = 				"END";
	s_keyNames[LJ_KEY_PAGEUP] = 			"PAGE UP";
	s_keyNames[LJ_KEY_PAGEDOWN] = 			"PAGE DOWN";
	
		// Function keys 
	s_keyNames[LJ_KEY_F1] = 				"F1";
	s_keyNames[LJ_KEY_F2] = 				"F2";
	s_keyNames[LJ_KEY_F3] = 				"F3";
	s_keyNames[LJ_KEY_F4] = 				"F4";
	s_keyNames[LJ_KEY_F5] = 				"F5";
	s_keyNames[LJ_KEY_F6] = 				"F6";
	s_keyNames[LJ_KEY_F7] = 				"F7";
	s_keyNames[LJ_KEY_F8] = 				"F8";
	s_keyNames[LJ_KEY_F9] = 				"F9";
	s_keyNames[LJ_KEY_F10] = 				"F10";
	s_keyNames[LJ_KEY_F11] = 				"F11";
	s_keyNames[LJ_KEY_F12] = 				"F12";
	s_keyNames[LJ_KEY_F13] = 				"F13";
	s_keyNames[LJ_KEY_F14] = 				"F14";
	s_keyNames[LJ_KEY_F15] = 				"F15";
	
		// modifier keys 
	s_keyNames[LJ_KEY_NUMLOCK] = 			"NUM LOCK";
	s_keyNames[LJ_KEY_CAPSLOCK] = 			"CAPS LOCK";
	s_keyNames[LJ_KEY_SCROLLOCK] = 			"SCROLL LOCK";
	s_keyNames[LJ_KEY_RSHIFT] = 			"RSHIFT";
	s_keyNames[LJ_KEY_LSHIFT] = 			"LSHIFT";
	s_keyNames[LJ_KEY_RCTRL] = 				"RCTRL";
	s_keyNames[LJ_KEY_LCTRL] = 				"LCTRL";
	s_keyNames[LJ_KEY_RALT] = 				"RALT";
	s_keyNames[LJ_KEY_LALT] = 				"LALT";
	s_keyNames[LJ_KEY_RMETA] = 				"RMETA";
	s_keyNames[LJ_KEY_LMETA] = 				"LMETA";
	s_keyNames[LJ_KEY_LWINDOWS] = 			"LWINDOWS";
	s_keyNames[LJ_KEY_RWINDOWS] = 			"RWINDOWS";
	s_keyNames[LJ_KEY_MODE] = 				"MODE";
	
	// Miscellaneous function keys 
	s_keyNames[LJ_KEY_HELP] = 				"HELP";
	s_keyNames[LJ_KEY_PRINT] = 				"PRINT";
	s_keyNames[LJ_KEY_SYSREQ] = 			"SYSREQ";
	s_keyNames[LJ_KEY_BREAK] = 				"BREAK";
	s_keyNames[LJ_KEY_MENU] = 				"MENU";

	s_keyNames[LJ_KEY_APPACTIVE] = 			"APPACTIVE";
	s_keyNames[LJ_KEY_QUIT] = 				"QUIT";

	LJ_keyReset();
}

void LJ_keyReset( void )
{
	int i;

	s_keyPrevState = &s_keyStateData[0][0];
	s_keyThisState = &s_keyStateData[1][0];

	s_keyModifierStatusPrev = &s_keyModifierStatusData[0];
	s_keyModifierStatusThis = &s_keyModifierStatusData[1];

	// Clear out this state
	for ( i = 0; i < LJ_NUM_KEYS; i++ )
	{
		s_keyThisState[i] = LJ_KEY_STATE_UNKNOWN;
		s_keyPrevState[i] = LJ_KEY_STATE_UNKNOWN;
	}

	*s_keyModifierStatusPrev = LJ_KEY_MOD_NONE;
	*s_keyModifierStatusThis = LJ_KEY_MOD_NONE;
}

void LJ_keyTick( void )
{
	int i;

	// Toggle the double buffer data 
	LJ_inputKeyStateEnum* const temp = s_keyPrevState;
	s_keyPrevState = s_keyThisState;
	s_keyThisState = temp;

	// Copy last frame state to this state
	for ( i = 0; i < LJ_NUM_KEYS; i++ )
	{
		s_keyThisState[i] = s_keyPrevState[i];
	}
}

void LJ_keyShutdown( void )
{
}

#if 0
// Valid keyboard modifiers 
enum 
{
	LJ_KEY_MOD_NONE  		= 0x0000,
	LJ_KEY_MOD_LSHIFT		= 0x0001,
	LJ_KEY_MOD_RSHIFT		= 0x0002,
	LJ_KEY_MOD_LCTRL 		= 0x0040,
	LJ_KEY_MOD_RCTRL 		= 0x0080,
	LJ_KEY_MOD_LALT  		= 0x0100,
	LJ_KEY_MOD_RALT  		= 0x0200,
	LJ_KEY_MOD_LMETA 		= 0x0400,
	LJ_KEY_MOD_RMETA 		= 0x0800,
	LJ_KEY_MOD_NUM   		= 0x1000,
	LJ_KEY_MOD_CAPS  		= 0x2000,
	LJ_KEY_MOD_MODE  		= 0x4000,
	LJ_KEY_MOD_RESERVED 	= 0x8000
} LJ_inputKeyModifierEnum;

#define LJ_KEY_MOD_CTRL		(KMOD_LCTRL|KMOD_RCTRL)
#define LJ_KEY_MOD_SHIFT	(KMOD_LSHIFT|KMOD_RSHIFT)
#define LJ_KEY_MOD_ALT		(KMOD_LALT|KMOD_RALT)
#define LJ_KEY_MOD_META		(KMOD_LMETA|KMOD_RMETA)

#endif // #if 0

char* const LJ_keyGetKeyName( const LJ_inputKeyEnum key )
{
	if ( ( key < LJ_KEY_START ) || ( key >= LJ_KEY_FINISH ) )
	{
		return "UNKNOWN";
	}
	return s_keyNames[key];
}

void LJ_keyUp( const LJ_inputKeyEnum key, const int modifier )
{
	s_keyThisState[key] = LJ_KEY_STATE_RELEASED;
	*s_keyModifierStatusThis = modifier;

	// TODO: Include modifiers up and down

	printf( "Key Up %d '%s'\n", key, LJ_keyGetKeyName( key ) );
}
 
void LJ_keyDown( const LJ_inputKeyEnum key, const int modifier )
{
	s_keyThisState[key] = LJ_KEY_STATE_PRESSED;
	*s_keyModifierStatusThis = modifier;

	// TODO: Include modifiers up and down

	printf( "Key Down %d '%s'\n", key, LJ_keyGetKeyName( key ) );
}
 
LJ_inputKeyStateEnum LJ_keyGetKeyThisState( const LJ_inputKeyEnum key )
{
	return s_keyThisState[key];
}

LJ_inputKeyStateEnum LJ_keyGetKeyPrevState( const LJ_inputKeyEnum key )
{
	return s_keyPrevState[key];
}

LJ_inputKeyModifierEnum LJ_keyGetKeyThisModifier( void )
{
	return *s_keyModifierStatusThis;
}

LJ_inputKeyModifierEnum LJ_keyGetKeyPrevModifier( void )
{
	return *s_keyModifierStatusPrev;
}

