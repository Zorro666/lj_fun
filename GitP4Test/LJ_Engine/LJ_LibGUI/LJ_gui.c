#include "LJ.h"
#include "LJ_gui.h"
#include "LJ_fltk.h"

void LJ_guiInit( LJ_int argc, LJ_char** argv )
{
	LJ_fltkInit( argc, argv );
}

void LJ_guiReset( void )
{
	LJ_fltkReset();
}

void LJ_guiShutdown( void )
{
	LJ_fltkShutdown();
}

void LJ_guiTick( void )
{
	LJ_fltkTick();
}

LJ_int LJ_guiMessageBox( const LJ_int numButtons, const LJ_char* const buttonTexts[], 
						 const LJ_char* const title, const LJ_char* const message )
{
	return LJ_fltkMessageBox( numButtons, buttonTexts, title, message );
}


