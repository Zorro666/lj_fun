#include "LJ.h"
#include "LJ_LibCommon/LJ_LibCommon.h"

#include "LJ_graphics.h"
#include "LJ_Texture/LJ_texture.h"

#include <GL/gl.h>

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

void LJ_cameraSetOrthographic( const LJ_float midX, const LJ_float midY, const LJ_float width, const LJ_float height )
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

	// Set the correct clipping range and orthographic projection
    glOrtho( -0.5f, +0.5f, -0.5f, +0.5f, -1.0f, 1.0f );
	// Then scale and translate to match user inputs
	glScalef( 1.0f/width, 1.0f/height, 1.0f );
	glTranslatef( -midX, -midY, 0.0f );

	// Switch back to modelview matrix mode
    glMatrixMode( GL_MODELVIEW );
}


