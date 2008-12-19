#ifndef DEGBUVAR_H
#define DEGBUVAR_H

struct debugVarDef_s;

enum debugVarInputEnum
{ 	
	DEBUG_VAR_INPUT_UP = 0, 
	DEBUG_VAR_INPUT_DOWN, 
	DEBUG_VAR_INPUT_LEFT, 
	DEBUG_VAR_INPUT_RIGHT, 
	DEBUG_VAR_INPUT_SELECT,
	DEBUG_VAR_INPUT_CANCEL,
	DEBUG_VAR_INPUT_FAST = 0x10000,
};

enum debugVarVarEnum
{
	DEBUG_VAR_INT = ( 1 << 0 ),
	DEBUG_VAR_FLOAT = ( 1 << 1 ),
	DEBUG_VAR_BOOL = ( 1 << 2 ),
	DEBUG_VAR_READ_ONLY = ( 1 << 3 ),
};

#ifdef __cplusplus
extern "C" 
{
#endif // #ifdef __cplusplus

void debugVarInit( const int maxNumVars );
void debugVarReset( void );
void debugVarShutdown( void );

int debugVarRegister( const char* const name, const int type, void* dataPtr, const int flags );
void debugVarRender( void );
void debugVarInput( const int debugInput );

// External functions that must be provided to make this system work
extern float debugVarRenderText( const int render, const float x, const float y, const unsigned int colour, const char* const outputString ); 
extern void* debugVarMemAlloc( const int sizeInBytes );
extern void debugVarMemFree( void* memoryPtr );
extern void debugVarDrawBackground( const float x0, const float y0, const float x1, const float y1, const unsigned int colour );

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus


#endif // #ifndef DEGBUVAR_H
