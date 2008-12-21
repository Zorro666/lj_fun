#include "LJ_input.h"
#include "LJ_key.h"

void keyInit( void );

void LJ_inputInit( void )
{
	keyInit();
}

void LJ_inputReset( void )
{
}

void LJ_inputShutdown( void )
{
}

int LJ_inputIsKeyPressed( const LJ_inputKey key )
{
	return 0;
}

int LJ_inputIsKeyRelease( const LJ_inputKey key )
{
	return 0;
}

int LJ_inputKeyClicked( const LJ_inputKey key )
{
	return 0;
}

