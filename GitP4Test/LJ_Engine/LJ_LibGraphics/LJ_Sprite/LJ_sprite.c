#include "LJ.h"
#include "LJ_sprite.h"

#include <GL/gl.h>

void LJ_spriteRender( const LJ_sprite* const sprite )
{
	const LJ_uint colour = sprite->rgba;
	const LJ_textureHandle th = sprite->texHandle;
	const LJ_float x = sprite->x;
	const LJ_float y = sprite->y;
	const LJ_float z = 0.0f;
	const LJ_float width = sprite->width;
	const LJ_float height = sprite->height;
	const LJ_float u0 = sprite->u0;
	const LJ_float v0 = sprite->v0;
	const LJ_float u1 = sprite->u1;
	const LJ_float v1 = sprite->v1;

	const LJ_float red = (LJ_float)(( colour >> 24 ) & 0xFF) / 255.0f;
	const LJ_float green = (LJ_float)(( colour >> 16 ) & 0xFF) / 255.0f;
	const LJ_float blue = (LJ_float)(( colour >> 8 ) & 0xFF) / 255.0f;
	const LJ_float alpha = (LJ_float)(( colour >> 0 ) & 0xFF) / 255.0f;

	glDepthMask( GL_FALSE );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glBindTexture( GL_TEXTURE_2D, th );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glColor4f( red, green, blue, alpha );
	glTranslatef( x, y, z );
	// 0--->1
	// |    |
	// |    |
	// 2--->3
	glBegin( GL_TRIANGLE_STRIP );
	glTexCoord2f( u0, v0 );
	glVertex3f( 0.0f, 0.0f, 0.0f );

	glTexCoord2f( u1, v0 );
	glVertex3f( width, 0.0f, 0.0f );

	glTexCoord2f( u0, v1 );
	glVertex3f( 0.0f, height, 0.0f );

	glTexCoord2f( u1, v1 );
	glVertex3f( width, height, 0.0f );

	glEnd();
    glPopMatrix();
}

