#ifndef LJ_DEGBUVAR_H
#define LJ_DEGBUVAR_H

#include "LJ_LibCommon/LJ_LibCommon.h"

#ifndef LJ_DEBUGVAR_MAX_NUM
#define LJ_DEBUGVAR_MAX_NUM 64
#endif // #ifndef LJ_DEBUGVAR_MAX_NUM

struct LJ_debugVarDef_s;

enum LJ_debugVarInputEnum
{ 	
	LJ_DEBUG_VAR_INPUT_UP = 0, 
	LJ_DEBUG_VAR_INPUT_DOWN, 
	LJ_DEBUG_VAR_INPUT_LEFT, 
	LJ_DEBUG_VAR_INPUT_RIGHT, 
	LJ_DEBUG_VAR_INPUT_SELECT,
	LJ_DEBUG_VAR_INPUT_CANCEL,
	LJ_DEBUG_VAR_INPUT_FAST = 0x10000,
};

enum LJ_debugVarVarEnum
{
	LJ_DEBUG_VAR_INT = ( 1 << 0 ),
	LJ_DEBUG_VAR_FLOAT = ( 1 << 1 ),
	LJ_DEBUG_VAR_BOOL = ( 1 << 2 ),
	LJ_DEBUG_VAR_READ_ONLY = ( 1 << 3 ),
};

void LJ_debugVarInit( const LJ_int maxNumVars );
void LJ_debugVarReset( void );
void LJ_debugVarShutdown( void );

LJ_int LJ_debugVarRegister( const LJ_char* const name, const LJ_int type, void* dataPtr, const LJ_int flags );
void LJ_debugVarRender( void );
void LJ_debugVarInput( const LJ_int debugInput );

// External functions that must be provided to make this system work
extern LJ_float LJ_debugVarRenderText( const LJ_int render, const LJ_float x, const LJ_float y, const LJ_uint colour, const LJ_char* const outputString ); 
extern void* LJ_debugVarMemAlloc( const LJ_int sizeInBytes );
extern void LJ_debugVarMemFree( void* memoryPtr );
extern void LJ_debugVarDrawBackground( const LJ_float x0, const LJ_float y0, const LJ_float x1, const LJ_float y1, const LJ_uint colour );

#endif // #ifndef LJ_DEGBUVAR_H
