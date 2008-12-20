#ifndef LJ_DEGBUVAR_H
#define LJ_DEGBUVAR_H

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

void LJ_debugVarInit( const int maxNumVars );
void LJ_debugVarReset( void );
void LJ_debugVarShutdown( void );

int LJ_debugVarRegister( const char* const name, const int type, void* dataPtr, const int flags );
void LJ_debugVarRender( void );
void LJ_debugVarInput( const int debugInput );

// External functions that must be provided to make this system work
extern float LJ_debugVarRenderText( const int render, const float x, const float y, const unsigned int colour, const char* const outputString ); 
extern void* LJ_debugVarMemAlloc( const int sizeInBytes );
extern void LJ_debugVarMemFree( void* memoryPtr );
extern void LJ_debugVarDrawBackground( const float x0, const float y0, const float x1, const float y1, const unsigned int colour );

#endif // #ifndef LJ_DEGBUVAR_H
