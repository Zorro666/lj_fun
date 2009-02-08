#ifndef LJ_FLTK_HH
#define LJ_FLTK_HH

#include "LJ_LibCommon/LJ_LibCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif // #ifdef __cpluscplus

void LJ_fltkInit( LJ_int argc, LJ_char** argv );
void LJ_fltkTick( void );
void LJ_fltkReset( void );
void LJ_fltkShutdown( void );

LJ_int LJ_fltkMessageBox( const LJ_int numButtons, const LJ_char* const buttonTexts[], 
						  const LJ_char* const title, const LJ_char* const message );

#ifdef __cplusplus
}
#endif // #ifdef __cpluscplus

#endif // #ifndef LJ_FLTK_HH

