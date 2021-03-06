#include "LJ.h"

#include "LJ_LibCommon/LJ_LibCommon.h"

#include "LJ_LibEngine/LJ_LibEngine.h"
#include "LJ_LibDebug/LJ_LibDebug.h"
#include "LJ_LibInput/LJ_LibInput.h"
#include "LJ_LibUnitTest/LJ_LibUnittest.h"
#include "LJ_LibGraphics/LJ_LibGraphics.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <time.h>

// For atexit
#include <stdlib.h>

// TODO: make use of SDL lib be on a #define and hide it away even more so the code can be ported to other platforms e.g. iphone/wii
LJ_int winWidth = 768;
LJ_int winHeight = 512;

SDL_Surface* s_sdlScreen;
GLUquadricObj* s_quadratic;

LJ_int s_debugVarEnabled = 0;

extern LJ_int s_sleepTime;

extern LJ_float s_cameraFoV;
extern LJ_float s_cameraX;
extern LJ_float s_cameraY;
extern LJ_float s_cameraZ;

extern LJ_int s_quit;
extern LJ_int s_minimized;

void renderBitmapString( LJ_float x, LJ_float y, LJ_float z, void *font, const LJ_char* const string );
// TODO: this is very hacky
void LJ_engineInputUpdate( void );

void LJ_engineEarlyInit( LJ_int argc, LJ_char* argv[] )
{
	atexit( gameExitFunction );

	// Initialize SDL
	SDL_Init( SDL_INIT_EVERYTHING );
 
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    // Initialize the GLUT environment - needed for font rendering
    glutInit( &argc, argv );

	s_sdlScreen = SDL_SetVideoMode( winWidth, winHeight, 32, SDL_OPENGL );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport( 0, 0, winWidth, winHeight );

    glOrtho( 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f );   // Many ways to do this
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glLineWidth( 1.0f );
    glEnable( GL_DEPTH_TEST );

	s_quadratic = gluNewQuadric();			
	gluQuadricNormals( s_quadratic, GLU_SMOOTH );
	gluQuadricTexture( s_quadratic, GL_TRUE );

	// TODO: this should be LJ_commonInit();
	LJ_typesInit();
	LJ_memInit();
	LJ_filesystemInit();
	// TODO: this needs to seeded in a better way!
	LJ_mathSeedRand( 345 );

	LJ_unittestInit();
	LJ_debugVarInit( LJ_DEBUGVAR_MAX_NUM );
	LJ_inputInit();
	LJ_graphicsInit();
}

void LJ_engineLateInit( void )
{
	// Run the unittests
	LJ_unittestTick();
}

//	long lastTick = SDL_GetTicks();

void LJ_engineReset( void )
{
	LJ_graphicsReset();
	LJ_inputReset();
	LJ_debugVarReset();
	LJ_unittestReset();
	// TODO: LJ_commonReset();
	LJ_filesystemReset();
}

void LJ_engineShutdown( void )
{
	LJ_graphicsShutdown();
	LJ_inputShutdown();
	LJ_debugVarShutdown();

	// TODO: LJ_commonShutdown();
	LJ_filesystemShutdown();
	LJ_memShutdown();

	SDL_Quit();
}

void LJ_engineTick( void )
{
	// TODO: remove the #if PLATFORM code need a sleep function
#if PLATFORM_WIN
	Sleep( s_sleepTime );
#endif // #if PLATFORM_WIN

#if PLATFORM_LINUX
	struct timespec rqtp;
	struct timespec rmtp;
#define MILLISECONDS (1000*1000)
	rqtp.tv_sec = 0;
	rqtp.tv_nsec = s_sleepTime * MILLISECONDS;
#undef MILLISECONDS
	nanosleep( &rqtp, &rmtp );
#endif	// #if PLATFORM_LINUX

	// Handle mouse and keyboard input
	LJ_inputTick();
	LJ_engineInputUpdate();
}

void LJ_engineStartRendering( void )
{
    // Clear the screen
    glClearColor( 1.0f, 0.5f, 0.8f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void LJ_engineFinishRendering( void )
{
    glFlush(); // Flush the buffer
    SDL_GL_SwapBuffers();
}

void LJ_engineRender( void )
{
    // Set the drawing color (RGB: WHITE)
    glColor3f( 1.0f, 1.0f, 1.0f );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

	// Calculate The Aspect Ratio Of The Window
	gluPerspective( s_cameraFoV, (GLfloat)winWidth/ (GLfloat)winHeight, 0.1f, 100.0f );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glTranslatef( s_cameraX, s_cameraY, s_cameraZ );

	game3DRender();

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport( 0, 0, winWidth, winHeight );

	// Want the coords to be 0,0 in top-left, 1,1 in bottom-right of the window
	// Set the correct clipping range and orthographic projection
	LJ_cameraSetOrthographic( 0.5f, 0.5f, 1.0f, 1.0f );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	game2DRender();

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0, 120, 0, 20 );
    glMatrixMode( GL_MODELVIEW );

	if ( s_debugVarEnabled )
	{
    	LJ_debugVarRender();
	}
}

// Return the width of the text rendered
LJ_float LJ_debugVarRenderText( const LJ_int render, const LJ_float x, const LJ_float y, const LJ_uint colour, const LJ_char* const outputString )
{
	const LJ_float multiplier = 2.8f * ( 512.0f / (LJ_float)winWidth );
	const LJ_int len = LJ_strGetLength( outputString );
	if ( render == 1 )
	{
		LJ_float new_x = x;
		LJ_float new_y = 19 - y;
		void* bitmapFont = GLUT_BITMAP_TIMES_ROMAN_24;
		const LJ_int red = ( colour >> 24 ) & 0xFF;
		const LJ_int green = ( colour >> 16 ) & 0xFF;
		const LJ_int blue = ( colour >> 8 ) & 0xFF;
		glDisable( GL_DEPTH_TEST );
		glDisable( GL_TEXTURE_2D );
		glColor3f( red/255.0f, green/255.0f, blue/255.0f );
		renderBitmapString( new_x, new_y, -0.5f, bitmapFont, outputString );
	}
	return len * multiplier;
}

void LJ_debugVarDrawBackground( const LJ_float x0, const LJ_float y0, const LJ_float x1, const LJ_float y1, const LJ_uint colour )
{
	const LJ_int red = ( colour >> 24 ) & 0xFF;
	const LJ_int green = ( colour >> 16 ) & 0xFF;
	const LJ_int blue = ( colour >> 8 ) & 0xFF;
	glColor3f( red/255.0f, green/255.0f, blue/255.0f );
	glDisable( GL_DEPTH_TEST );
    glPushMatrix();
	glBegin( GL_QUADS );
    glVertex3f( x0, 20 - y0, 0.0f );
    glVertex3f( x1, 20 - y0, 0.0f );
    glVertex3f( x1, 20 - y1, 0.0f );
    glVertex3f( x0, 20 - y1, 0.0f );
	glEnd();
	glColor3f( 1.0f, 1.0f, 1.0f );
	glBegin( GL_LINE_STRIP ); 
    glVertex3f( x0, 20 - y0, 0.0f );
    glVertex3f( x1, 20 - y0, 0.0f );
    glVertex3f( x1, 20 - y1, 0.0f );
    glVertex3f( x0, 20 - y1, 0.0f );
    glVertex3f( x0, 20 - y0, 0.0f );
	glEnd();
	glPopMatrix();
}

void* LJ_debugVarMemAlloc( const LJ_int sizeInBytes )
{
	return malloc( sizeInBytes );
}

void LJ_debugVarMemFree( void* memoryPtr )
{
	free( memoryPtr );
}

void renderBitmapString( LJ_float x, LJ_float y, LJ_float z, void *font, const LJ_char* const string ) 
{  
    const LJ_char* c;
    glPushMatrix();
    glRasterPos3f( x, y, z );
    for ( c = string; *c != '\0'; c++ ) 
    {
        glutBitmapCharacter( font, *c );
    }
    glPopMatrix();
}

void LJ_debugDrawSphere( const LJ_float x, const LJ_float y, const LJ_float z, const LJ_float radius, const LJ_uint colour )
{
	const LJ_float red = (LJ_float)(( colour >> 24 ) & 0xFF) / 255.0f;
	const LJ_float green = (LJ_float)(( colour >> 16 ) & 0xFF) / 255.0f;
	const LJ_float blue = (LJ_float)(( colour >> 8 ) & 0xFF) / 255.0f;

	glDisable( GL_TEXTURE_2D );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
	glColor4f( red, green, blue, 0.5f );
	glTranslatef( x, y, z );
	gluSphere( s_quadratic, radius, 32, 32 );
    glPopMatrix();
}

void LJ_debugDrawCircle( const LJ_float x, const LJ_float y, const LJ_float z, const LJ_float radius, const LJ_uint colour )
{
	const LJ_float red = (LJ_float)(( colour >> 24 ) & 0xFF) / 255.0f;
	const LJ_float green = (LJ_float)(( colour >> 16 ) & 0xFF) / 255.0f;
	const LJ_float blue = (LJ_float)(( colour >> 8 ) & 0xFF) / 255.0f;
	const LJ_float alpha = (LJ_float)(( colour >> 0 ) & 0xFF) / 255.0f;

	glDisable( GL_TEXTURE_2D );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
	glColor4f( red, green, blue, alpha );
	glTranslatef( x, y, z );
	gluDisk( s_quadratic, radius-1.01f, radius, 64, 64 );
    glPopMatrix();
}

void LJ_debugDrawQuadTexture( const LJ_float x, const LJ_float y, const LJ_float z, 
						      const LJ_float width, const LJ_float height, const LJ_textureHandle th, const LJ_uint colour )
{
	const LJ_float red = (LJ_float)(( colour >> 24 ) & 0xFF) / 255.0f;
	const LJ_float green = (LJ_float)(( colour >> 16 ) & 0xFF) / 255.0f;
	const LJ_float blue = (LJ_float)(( colour >> 8 ) & 0xFF) / 255.0f;
	const LJ_float alpha = (LJ_float)(( colour >> 0 ) & 0xFF) / 255.0f;

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, th );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
	glColor4f( red, green, blue, alpha );
	glTranslatef( x-width/2, y-height/2, z );
	// 0--->1
	// |    |
	// |    |
	// 2--->3
	glBegin( GL_TRIANGLE_STRIP );
	glTexCoord2f( 0.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );

	glTexCoord2f( 1.0f, 0.0f );
	glVertex3f( width, 0.0f, 0.0f );

	glTexCoord2f( 0.0f, 1.0f );
	glVertex3f( 0.0f, height, 0.0f );

	glTexCoord2f( 1.0f, 1.0f );
	glVertex3f( width, height, 0.0f );

	glEnd();
    glPopMatrix();
}

void LJ_engineInputUpdate( void )
{
	const LJ_inputKeyModifier modifier = LJ_inputGetKeyModifier();
	LJ_int fastKey = 0;

	if ( ( LJ_inputKeyClicked( LJ_KEY_ESCAPE ) ) || ( LJ_inputKeyClicked( LJ_KEY_q ) ) )
	{
		s_quit = 1;
	}

	if ( modifier & LJ_KEY_MOD_CTRL )
	{
		fastKey = LJ_DEBUG_VAR_INPUT_FAST;
	}
	if ( ( LJ_inputKeyClicked( LJ_KEY_UP ) ) || ( LJ_inputKeyClicked( LJ_KEY_w ) ) )
	{
		LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_UP | fastKey );
	}
	if ( ( LJ_inputKeyClicked( LJ_KEY_DOWN ) ) || ( LJ_inputKeyClicked( LJ_KEY_s ) ) )
	{
		LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_DOWN | fastKey );
	}
	if ( ( LJ_inputKeyClicked( LJ_KEY_LEFT ) ) || ( LJ_inputKeyClicked( LJ_KEY_a ) ) )
	{
		LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_LEFT | fastKey );
	}
	if ( ( LJ_inputKeyClicked( LJ_KEY_RIGHT ) ) || ( LJ_inputKeyClicked( LJ_KEY_d ) ) )
	{
		LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_RIGHT | fastKey );
	}
	if ( LJ_inputKeyClicked( LJ_KEY_SPACE ) )
	{
		LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_SELECT | fastKey );
	}
	if ( LJ_inputKeyClicked( LJ_KEY_BACKSPACE ) )
	{
		LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_CANCEL | fastKey );
	}
	if ( LJ_inputMouseButtonClicked( LJ_MOUSE_BUTTON_RIGHT ) )
	{
		LJ_outputPrintDebug( ( "Right button clicked - outputDebug\n" ) );
		s_debugVarEnabled ^= 1;
	}
/*
    switch ( symKey ) 
	{
        case 'r':
		{
            LJ_debugVarRender();
            break;
		}
    }
*/
}
 
