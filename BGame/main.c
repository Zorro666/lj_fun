// main.c : defines the entry point for the console application.
//
// Test commit for RSS feed into Thunderbird

#include <SDL/SDL.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "LJ_LibEngine/LJ_LibEngine.h"
#include "LJ_LibDebug/LJ_LibDebug.h"

float s_cameraFoV = 60.0f;
float s_cameraX = 0.0f;
float s_cameraY = 0.0f;
float s_cameraZ = -10.0f;
int s_sleepTime = 100;

int s_quit = 0;
int s_minimized = 0;

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
	LJ_debugDrawCircle( 0.5f, 0.5f, 0.0f, 0.2f, 0x00FF00FF );
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
    LJ_debugVarInit( 64 );
    LJ_debugVarRegister( "Camera:x", LJ_DEBUG_VAR_FLOAT, &s_cameraX, 0 );
    LJ_debugVarRegister( "Camera:y", LJ_DEBUG_VAR_FLOAT, &s_cameraY, 0 );
    LJ_debugVarRegister( "Camera:z", LJ_DEBUG_VAR_FLOAT, &s_cameraZ, 0 );
    LJ_debugVarRegister( "Camera:fov", LJ_DEBUG_VAR_FLOAT, &s_cameraFoV, 0 );
    LJ_debugVarRegister( "Main:sleepTime", LJ_DEBUG_VAR_INT, &s_sleepTime, 0 );
}

void gameReset( void )
{
	LJ_debugVarReset();
}

void gameShutdown( void )
{
    LJ_debugVarShutdown();
}

int main(int argc, char* argv[])
{
    LJ_engineInit( argc, argv );
	gameInit();

	while ( !s_quit )
	{
		gameSingleLoop();
	}

	gameShutdown();
	LJ_engineShutdown();

    return 0;
}

