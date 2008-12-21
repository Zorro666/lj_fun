#ifndef LJ_KEY_HH
#define LJ_KEY_HH

enum 
{
	LJ_KEY_START			= 0,
	// The key IDs chosen to match ASCII
	LJ_KEY_UNKNOWN			= 0,
	LJ_KEY_BACKSPACE		= 8,
	LJ_KEY_TAB				= 9,
	LJ_KEY_CLEAR			= 12,
	LJ_KEY_RETURN			= 13,
	LJ_KEY_PAUSE			= 19,
	LJ_KEY_ESCAPE			= 27,
	LJ_KEY_SPACE			= 32,
	LJ_KEY_EXCLAIM			= 33,
	LJ_KEY_QUOTEDBL			= 34,
	LJ_KEY_HASH				= 35,
	LJ_KEY_DOLLAR			= 36,
	LJ_KEY_AMPERSAND		= 38,
	LJ_KEY_QUOTE			= 39,
	LJ_KEY_LEFTPAREN		= 40,
	LJ_KEY_RIGHTPAREN		= 41,
	LJ_KEY_ASTERISK			= 42,
	LJ_KEY_PLUS				= 43,
	LJ_KEY_COMMA			= 44,
	LJ_KEY_MINUS			= 45,
	LJ_KEY_PERIOD			= 46,
	LJ_KEY_SLASH			= 47,
	LJ_KEY_0				= 48,
	LJ_KEY_1				= 49,
	LJ_KEY_2				= 50,
	LJ_KEY_3				= 51,
	LJ_KEY_4				= 52,
	LJ_KEY_5				= 53,
	LJ_KEY_6				= 54,
	LJ_KEY_7				= 55,
	LJ_KEY_8				= 56,
	LJ_KEY_9				= 57,
	LJ_KEY_COLON			= 58,
	LJ_KEY_SEMICOLON		= 59,
	LJ_KEY_LESS				= 60,
	LJ_KEY_EQUALS			= 61,
	LJ_KEY_GREATER			= 62,
	LJ_KEY_QUESTION			= 63,
	LJ_KEY_AT				= 64,

	//   Skip uppercase letters
	LJ_KEY_LEFTBRACKET		= 91,
	LJ_KEY_BACKSLASH		= 92,
	LJ_KEY_RIGHTBRACKET		= 93,
	LJ_KEY_CARET			= 94,
	LJ_KEY_UNDERSCORE		= 95,
	LJ_KEY_BACKQUOTE		= 96,
	LJ_KEY_a				= 97,
	LJ_KEY_b				= 98,
	LJ_KEY_c				= 99,
	LJ_KEY_d				= 100,
	LJ_KEY_e				= 101,
	LJ_KEY_f				= 102,
	LJ_KEY_g				= 103,
	LJ_KEY_h				= 104,
	LJ_KEY_i				= 105,
	LJ_KEY_j				= 106,
	LJ_KEY_k				= 107,
	LJ_KEY_l				= 108,
	LJ_KEY_m				= 109,
	LJ_KEY_n				= 110,
	LJ_KEY_o				= 111,
	LJ_KEY_p				= 112,
	LJ_KEY_q				= 113,
	LJ_KEY_r				= 114,
	LJ_KEY_s				= 115,
	LJ_KEY_t				= 116,
	LJ_KEY_u				= 117,
	LJ_KEY_v				= 118,
	LJ_KEY_w				= 119,
	LJ_KEY_x				= 120,
	LJ_KEY_y				= 121,
	LJ_KEY_z				= 122,
	LJ_KEY_DELETE			= 127,
	// End of ASCII mapped keys

	// Numeric keypad 
	LJ_KEY_NUMPAD0			= 256,
	LJ_KEY_NUMPAD1			= 257,
	LJ_KEY_NUMPAD2			= 258,
	LJ_KEY_NUMPAD3			= 259,
	LJ_KEY_NUMPAD4			= 260,
	LJ_KEY_NUMPAD5			= 261,
	LJ_KEY_NUMPAD6			= 262,
	LJ_KEY_NUMPAD7			= 263,
	LJ_KEY_NUMPAD8			= 264,
	LJ_KEY_NUMPAD9			= 265,
	LJ_KEY_NUMPAD_PERIOD	= 266,
	LJ_KEY_NUMPAD_DIVIDE	= 267,
	LJ_KEY_NUMPAD_MULTIPLY	= 268,
	LJ_KEY_NUMPAD_MINUS		= 269,
	LJ_KEY_NUMPAD_PLUS		= 270,
	LJ_KEY_NUMPAD_ENTER		= 271,
	LJ_KEY_NUMPAD_EQUALS	= 272,

	// Arrows + Home/End pad 
	LJ_KEY_UP				= 273,
	LJ_KEY_DOWN				= 274,
	LJ_KEY_RIGHT			= 275,
	LJ_KEY_LEFT				= 276,
	LJ_KEY_INSERT			= 277,
	LJ_KEY_HOME				= 278,
	LJ_KEY_END				= 279,
	LJ_KEY_PAGEUP			= 280,
	LJ_KEY_PAGEDOWN			= 281,

	// Function keys 
	LJ_KEY_F1				= 282,
	LJ_KEY_F2				= 283,
	LJ_KEY_F3				= 284,
	LJ_KEY_F4				= 285,
	LJ_KEY_F5				= 286,
	LJ_KEY_F6				= 287,
	LJ_KEY_F7				= 288,
	LJ_KEY_F8				= 289,
	LJ_KEY_F9				= 290,
	LJ_KEY_F10				= 291,
	LJ_KEY_F11				= 292,
	LJ_KEY_F12				= 293,
	LJ_KEY_F13				= 294,
	LJ_KEY_F14				= 295,
	LJ_KEY_F15				= 296,

	// modifier keys 
	LJ_KEY_NUMLOCK			= 300,
	LJ_KEY_CAPSLOCK			= 301,
	LJ_KEY_SCROLLOCK		= 302,
	LJ_KEY_RSHIFT			= 303,
	LJ_KEY_LSHIFT			= 304,
	LJ_KEY_RCTRL			= 305,
	LJ_KEY_LCTRL			= 306,
	LJ_KEY_RALT				= 307,
	LJ_KEY_LALT				= 308,
	LJ_KEY_RMETA			= 309,
	LJ_KEY_LMETA			= 310,
	LJ_KEY_LSUPER			= 311,		/* Left "Windows" key */
	LJ_KEY_RSUPER			= 312,		/* Right "Windows" key */
	LJ_KEY_MODE				= 313,		/* "Alt Gr" key */

	// Miscellaneous function keys 
	LJ_KEY_HELP				= 315,
	LJ_KEY_PRINT			= 316,
	LJ_KEY_SYSREQ			= 317,
	LJ_KEY_BREAK			= 318,
	LJ_KEY_MENU				= 319,

	LJ_KEY_FINISH
} LJ_inputKeyEnum;

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

#endif // #ifndef LJ_KEY_HH

