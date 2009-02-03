#include "LJ.h"
#include "LJ_LibCommon/LJ_LibCommon.h"

#include "LJ_graphics.h"
#include "LJ_Texture/LJ_texture.h"

void LJ_graphicsInit( void )
{
	LJ_textureInit();
}

void LJ_graphicsReset( void )
{
	LJ_textureReset();
}

void LJ_graphicsShutdown( void )
{
	LJ_textureShutdown();
}


