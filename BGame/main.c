// main.c : defines the entry point for the console application.
//
// Test commit for RSS feed into Thunderbird

#include <SDL/SDL.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "engine.h"
#include "DebugVar/debugVar.h"

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

void mainLoop()
{
//	long lastTick = SDL_GetTicks();
 
	// Main loop: loop forever.
	while ( !s_quit )
	{
		// Handle mouse and keyboard input
		engineHandleInput();
 
		if ( s_minimized ) 
		{
			// Release some system resources if the application is minimized.
			WaitMessage();
		} 
		else 
		{
			engineTick();
			engineRender();
		}
	}
}


int main(int argc, char* argv[])
{
    engineInit( argc, argv );

	// Game init
    debugVarInitialise( 64 );
    debugVarRegister( "Camera:x", DEBUG_VAR_FLOAT, &s_cameraX, 0 );
    debugVarRegister( "Camera:y", DEBUG_VAR_FLOAT, &s_cameraY, 0 );
    debugVarRegister( "Camera:z", DEBUG_VAR_FLOAT, &s_cameraZ, 0 );
    debugVarRegister( "Camera:fov", DEBUG_VAR_FLOAT, &s_cameraFoV, 0 );
    debugVarRegister( "Main:sleepTime", DEBUG_VAR_INT, &s_sleepTime, 0 );

	mainLoop();

    debugVarDeInitialise();
	engineShutdown();

    return 0;
}

