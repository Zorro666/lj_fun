#ifndef LJ_MOUSE_H
#define LJ_MOUSE_H

typedef enum LJ_inputMouseButtonEnum
{
	LJ_MOUSE_BUTTON_START 			= 0,
	LJ_MOUSE_BUTTON_UNKNOWN 		= 0,
	LJ_MOUSE_BUTTON_LEFT 			= 1,
	LJ_MOUSE_BUTTON_MIDDLE 			= 2,
	LJ_MOUSE_BUTTON_RIGHT 			= 3,
	LJ_MOUSE_BUTTON_WHEELUP 		= 4,
	LJ_MOUSE_BUTTON_WHEELDOWN 		= 5,

	LJ_MOUSE_BUTTON_FINISH
} LJ_inputMouseButtonEnum;

typedef enum LJ_inputMouseButtonStateEnum
{
	LJ_MOUSE_BUTTON_STATE_UNKNOWN 	= 0,
	LJ_MOUSE_BUTTON_STATE_PRESSED 	= 1,
	LJ_MOUSE_BUTTON_STATE_RELEASED 	= 2
} LJ_inputMouseButtonStateEnum;

#endif // #ifndef LJ_MOUSE_H

