// main.c : defines the entry point for the console application.
//
// Test commit for RSS feed into Thunderbird

#include "LJ.h"
#include "LJ_LibCommon/LJ_LibCommon.h"

#include "LJ_LibEngine/LJ_LibEngine.h"
#include "LJ_LibDebug/LJ_LibDebug.h"
#include "LJ_LibInput/LJ_LibInput.h"

LJ_float s_cameraFoV = 60.0f;
LJ_float s_cameraX = 0.0f;
LJ_float s_cameraY = 0.0f;
LJ_float s_cameraZ = -10.0f;
LJ_int s_sleepTime = 100;

LJ_int s_quit = 0;
LJ_int s_minimized = 0;

// pause the application until focus in regained
void WaitMessage( void )
{
}

void gameTick( void )
{
}

void game3DRender( void )
{
	LJ_debugDrawSphere( 0.0f, 0.0f, -10.0f, 3.0f, 0xFF0000FF );
}

void game2DRender( void )
{
	LJ_int mouseX, mouseY;

	LJ_float x, y;
	LJ_float radius = 0.1f;
	const LJ_float WIN_WIDTH = 768;
	const LJ_float WIN_HEIGHT = 512;

	LJ_inputMouseGetPosition( &mouseX, &mouseY );

	x = (LJ_float)mouseX / WIN_WIDTH;
	y = 1.0f - (LJ_float)mouseY / WIN_HEIGHT;

	LJ_debugDrawCircle( x, y, 0.0f, radius, 0x00FF00FF );
}

void gameSingleLoop()
{
	if ( s_minimized ) 
	{
		// Release some system resources if the application is minimized.
		WaitMessage();
	} 
	else 
	{
		LJ_engineTick();
		gameTick();
		LJ_engineStartRendering();
		LJ_engineRender();
		LJ_engineFinishRendering();
	}
}

void gameInit( void )
{
	// Game init
    LJ_debugVarRegister( "Camera:x", LJ_DEBUG_VAR_FLOAT, &s_cameraX, 0 );
    LJ_debugVarRegister( "Camera:y", LJ_DEBUG_VAR_FLOAT, &s_cameraY, 0 );
    LJ_debugVarRegister( "Camera:z", LJ_DEBUG_VAR_FLOAT, &s_cameraZ, 0 );
    LJ_debugVarRegister( "Camera:fov", LJ_DEBUG_VAR_FLOAT, &s_cameraFoV, 0 );
    LJ_debugVarRegister( "Main:sleepTime", LJ_DEBUG_VAR_INT, &s_sleepTime, 0 );
}

void gameReset( void )
{
}

void gameShutdown( void )
{
}

LJ_int main(LJ_int argc, LJ_char* argv[])
{
    LJ_engineEarlyInit( argc, argv );
	gameInit();
	LJ_engineLateInit();

	while ( !s_quit )
	{
		gameSingleLoop();
	}

	gameShutdown();
	LJ_engineShutdown();

    return 0;
}

