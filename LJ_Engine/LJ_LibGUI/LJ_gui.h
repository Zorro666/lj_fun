#ifndef LJ_GUI_HH
#define LJ_GUI_HH

#include "LJ_LibCommon/LJ_LibCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif // #ifdef __cpluscplus

void LJ_guiInit( LJ_int argc, LJ_char** argv );
void LJ_guiTick( void );
void LJ_guiReset( void );
void LJ_guiShutdown( void );

LJ_int LJ_guiMessageBox( const LJ_int numButtons, const LJ_char* const buttonTexts[], 
						 const LJ_char* const title, const LJ_char* const message );

#ifdef __cplusplus
}
#endif // #ifdef __cpluscplus

#endif // #ifndef LJ_GUI_HH
