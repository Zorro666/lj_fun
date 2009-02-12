// BG_main.c : main file for the BGame project

#include "LJ.h"
#include "LJ_LibCommon/LJ_LibCommon.h"

#include "LJ_LibEngine/LJ_LibEngine.h"
#include "LJ_LibDebug/LJ_LibDebug.h"
#include "LJ_LibInput/LJ_LibInput.h"
#include "LJ_LibGUI/LJ_LibGUI.h"

LJ_float s_cameraFoV = 60.0f;
LJ_float s_cameraX = 0.0f;
LJ_float s_cameraY = 0.0f;
LJ_float s_cameraZ = -10.0f;
LJ_int s_sleepTime = 100;

LJ_int s_quit = 0;
LJ_int s_minimized = 0;

LJ_textureHandle bgTex = LJ_TEXTURE_HANDLE_INVALID;
LJ_sprite bgSprite;

LJ_textureHandle fgTex = LJ_TEXTURE_HANDLE_INVALID;
LJ_sprite fgSprite;

extern LJ_float g_width;
extern LJ_float g_height;

LJ_float g_width = 1.0f;
LJ_float g_height = 1.0f;

typedef struct BG_Entity
{
	LJ_float x;
	LJ_float y;
	LJ_sprite* sprite;
} BG_Entity;

BG_Entity player;
BG_Entity camera;

LJ_textureHandle playerTex = LJ_TEXTURE_HANDLE_INVALID;
LJ_sprite playerSprite;

const LJ_float WIN_WIDTH = 768;
const LJ_float WIN_HEIGHT = 512;

void game3DRender( void )
{
	LJ_debugDrawSphere( 0.0f, 0.0f, -10.0f, 3.0f, 0xFF0000FF );
}

// Coords are 0,0 in bottom-left, 1,1 in top-right of the window
void game2DRender( void )
{
	LJ_int mouseX, mouseY;

	LJ_float x, y;
	LJ_float radius = 10.05f;
	const LJ_float ourWidth = WIN_WIDTH;
	const LJ_float ourHeight = ourWidth * ( WIN_HEIGHT / WIN_WIDTH );

	LJ_cameraSetOrthographic( camera.x, camera.y, ourWidth, ourHeight );

	// Draw the bg
	LJ_spriteRender( &bgSprite );

	// Draw the player
	player.sprite->x = player.x;
	player.sprite->y = player.y;
	LJ_spriteRender( player.sprite );

	// Draw the fg image
	LJ_spriteRender( &fgSprite );

	LJ_debugDrawCircle( -ourWidth/2.0f, -ourHeight/2.0f, 0.0f, radius, 0xFF0000FF );
	LJ_debugDrawCircle( 0.0f, 0.0f, 0.0f, radius, 0x00FF00FF );
	LJ_debugDrawCircle( ourWidth/2.0f, ourHeight/2.0f, 0.0f, radius, 0x0000FFFF );

	// Draw a mouse cursor
	LJ_inputMouseGetPosition( &mouseX, &mouseY );

	x = (LJ_float)mouseX / WIN_WIDTH;
	y = 1.0f - (LJ_float)mouseY / WIN_HEIGHT;
	x *= ourWidth;
	y *= ourHeight;
	x -= ourWidth / 2.0f;
	y -= ourHeight / 2.0f;
	x += camera.x;
	y += camera.y;

	LJ_debugDrawCircle( x, y, 0.0f, 20.02f, 0x00FFFFFF );
}

// pause the application until focus in regained
static void WaitMessage( void )
{
}

static void gameTick( void )
{
	LJ_int mouseX;
	LJ_int mouseY;
	const LJ_int mouseScrollLimit = ( ( WIN_WIDTH * 2 ) / 10 );
	const LJ_int mouseScrollMin = mouseScrollLimit;
	const LJ_int mouseScrollMax = WIN_WIDTH - mouseScrollLimit;
	const LJ_float cameraMinX = -500.0f;
	const LJ_float cameraMaxX = +500.0f;
	const LJ_float playerSpeedX = 0.3f;

	// Move the player
	if ( LJ_inputKeyPressed( LJ_KEY_RIGHT ) == LJ_TRUE )
	{
		player.x += playerSpeedX;
	}
	if ( LJ_inputKeyPressed( LJ_KEY_LEFT ) == LJ_TRUE )
	{
		player.x -= playerSpeedX;
	}

	// Move the camera
	LJ_inputMouseGetPosition( &mouseX, &mouseY );
	if ( mouseX < mouseScrollMin )
	{
		// Scroll-left
		camera.x -= 1.0f;
	}
	else if ( mouseX > mouseScrollMax )
	{
		// Scroll-right
		camera.x += 1.0f;
	}
	if ( camera.x < cameraMinX )
	{
		camera.x = cameraMinX;
	}
	if ( camera.x > cameraMaxX )
	{
		camera.x = cameraMaxX;
	}
}

static void gameSingleLoop( void )
{
	//LJ_assert( LJ_FALSE, ( "Test assert box which can be ignored" ) );
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

static void gameInit( void )
{
	const char* const bgFilename = "../../Data/BGame/bg.tga";
	const char* const fgFilename = "../../Data/BGame/fg.tga";
	const char* const playerFilename = "../../Data/BGame/player.tga";
	//const char* const playerFilename = "../test/test.tga";

	// Game init
    LJ_debugVarRegister( "Camera:x", LJ_DEBUG_VAR_FLOAT, &s_cameraX, 0 );
    LJ_debugVarRegister( "Camera:y", LJ_DEBUG_VAR_FLOAT, &s_cameraY, 0 );
    LJ_debugVarRegister( "Camera:z", LJ_DEBUG_VAR_FLOAT, &s_cameraZ, 0 );
    LJ_debugVarRegister( "Camera:fov", LJ_DEBUG_VAR_FLOAT, &s_cameraFoV, 0 );
    LJ_debugVarRegister( "Main:sleepTime", LJ_DEBUG_VAR_INT, &s_sleepTime, 0 );

	bgTex = LJ_textureLoadTGA( bgFilename, LJ_NULL );
	LJ_assert( bgTex != LJ_TEXTURE_HANDLE_INVALID, ( "Can't load TGA '%s'\n", bgFilename ) );
	bgSprite.texHandle = bgTex;
	bgSprite.x = -500.0f;
	bgSprite.y = 0.0f;
	bgSprite.width = 1000.0f;
	bgSprite.height = 300.0f;
	bgSprite.layer = LJ_SPRITE_BG;
	bgSprite.priority = 0;
	bgSprite.rgba = 0xFFFFFFFF;
	bgSprite.u0 = 0.0f;
	bgSprite.v0 = 0.0f;
	bgSprite.u1 = 1.0f;
	bgSprite.v1 = 1.0f;

	fgTex = LJ_textureLoadTGA( fgFilename, LJ_NULL );
	LJ_assert( fgTex != LJ_TEXTURE_HANDLE_INVALID, ( "Can't load TGA '%s'\n", fgFilename ) );
	fgSprite.texHandle = fgTex;
	fgSprite.x = -500.0f;
	fgSprite.y = 0.0f;
	fgSprite.width = 1000.0f;
	fgSprite.height = 300.0f;
	fgSprite.layer = LJ_SPRITE_FG;
	fgSprite.priority = 0;
	fgSprite.rgba = 0xFFFFFFFF;
	fgSprite.u0 = 0.0f;
	fgSprite.v0 = 0.0f;
	fgSprite.u1 = 1.0f;
	fgSprite.v1 = 1.0f;

	player.x = 0.0f;
	player.y = 0.0f;
	player.sprite = &playerSprite;

	playerTex = LJ_textureLoadTGA( playerFilename, LJ_NULL );
	LJ_assert( playerTex != LJ_TEXTURE_HANDLE_INVALID, ( "Can't load TGA '%s'\n", playerFilename ) );
	playerSprite.texHandle = playerTex;
	playerSprite.width = 64;
	playerSprite.height = 64;
	playerSprite.layer = LJ_SPRITE_MAIN;
	playerSprite.priority = 0;
	playerSprite.rgba = 0xFFFFFFFF;
	playerSprite.u0 = 0.0f;
	playerSprite.v0 = 0.0f;
	playerSprite.u1 = 1.0f;
	playerSprite.v1 = 1.0f;

	camera.x = 0.0f;
	camera.y = 200.0f;
	camera.sprite = LJ_NULL;

	player.x = camera.x;
	player.y = camera.y - 100.0f;
}

static void gameShutdown( void )
{
}

void gameExitFunction( void )
{
	gameShutdown();
	LJ_engineShutdown();
}

LJ_int main(LJ_int argc, LJ_char* argv[])
{
    LJ_engineEarlyInit( argc, argv );
	gameInit();
	LJ_engineLateInit();

	LJ_guiInit( argc, argv );
	while ( !s_quit )
	{
		gameSingleLoop();
		LJ_guiTick();
	}

    return 0;
}

