// UN_main.c : main file for the Unittest project

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

void game3DRender( void )
{
}

void game2DRender( void )
{
}

static void gameInit( void )
{
}

static void gameShutdown( void )
{
}

static void gameTick( void )
{
}

static void gameSingleLoop( void )
{
	LJ_engineTick();
	gameTick();
	LJ_engineStartRendering();
	LJ_engineRender();
	LJ_engineFinishRendering();
}

LJ_int main(LJ_int argc, LJ_char* argv[])
{
    LJ_engineEarlyInit( argc, argv );
	gameInit();
	LJ_engineLateInit();

	gameSingleLoop();

	gameShutdown();
	LJ_engineShutdown();

    return 0;
}

