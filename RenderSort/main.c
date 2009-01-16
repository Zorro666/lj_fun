// main.c : defines the entry point for the console application.

#include "LJ.h"

#include "LJ_LibCommon/LJ_LibCommon.h"
#include "LJ_LibDebug/LJ_LibDebug.h"
#include "renderSort.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void* strokeFont = GLUT_STROKE_MONO_ROMAN;

int winWidth = 768;
int winHeight = 512;

int drawStrokeFont = 0;

GLUquadricObj* s_quadratic;
float s_cameraFoV = 60.0f;
float s_cameraX = 0.0f;
float s_cameraY = 0.0f;
float s_cameraZ = -10.0f;

int s_sleepTime = 100;
int s_quit = 0;
int s_minimized = 0;

void renderBitmapString( float x, float y, float z, void *font, const char* const string );

enum
{
	KEY_ESCAPE = 27,
	KEY_BACKSPACE = 8,
	KEY_UP = 273,
	KEY_DOWN = 274,
	KEY_RIGHT = 275,
	KEY_LEFT = 276,
};

// Return the width of the text rendered
float LJ_debugVarRenderText( const int render, const float x, const float y, const unsigned int colour, const char* const outputString )
{
	const float multiplier = 2.8f * ( 512.0f / (float)winWidth );
	const int len = strlen( outputString );
	if ( render == 1 )
	{
		float new_x = x;
		float new_y = 19 - y;
		void* bitmapFont = GLUT_BITMAP_TIMES_ROMAN_24;
		const int red = ( colour >> 24 ) & 0xFF;
		const int green = ( colour >> 16 ) & 0xFF;
		const int blue = ( colour >> 8 ) & 0xFF;
		glDisable( GL_DEPTH_TEST );
		glColor3f( red/255.0f, green/255.0f, blue/255.0f );
		renderBitmapString( new_x, new_y, -0.5f, bitmapFont, outputString );
	}
	return len * multiplier;
}

void LJ_debugVarDrawBackground( const float x0, const float y0, const float x1, const float y1, const unsigned int colour )
{
	const int red = ( colour >> 24 ) & 0xFF;
	const int green = ( colour >> 16 ) & 0xFF;
	const int blue = ( colour >> 8 ) & 0xFF;
	glColor3f( red/255.0f, green/255.0f, blue/255.0f );
	glDisable( GL_DEPTH_TEST );
    glPushMatrix();
	glBegin( GL_QUADS );
    glVertex3f(x0, 20 - y0, 0.0f );
    glVertex3f(x1, 20 - y0, 0.0f );
    glVertex3f(x1, 20 - y1, 0.0f );
    glVertex3f(x0, 20 - y1, 0.0f );
	glEnd();
	glColor3f( 1.0f, 1.0f, 1.0f );
	glBegin( GL_LINE_STRIP ); 
    glVertex3f(x0, 20 - y0, 0.0f );
    glVertex3f(x1, 20 - y0, 0.0f );
    glVertex3f(x1, 20 - y1, 0.0f );
    glVertex3f(x0, 20 - y1, 0.0f );
    glVertex3f(x0, 20 - y0, 0.0f );
	glEnd();
	glPopMatrix();
}

void* LJ_debugVarMemAlloc( const int sizeInBytes )
{
	return malloc( sizeInBytes );
}

void LJ_debugVarMemFree( void* memoryPtr )
{
	free( memoryPtr );
}


void renderStrokeFontString( float x, float y, float z, void *font, char *string )
{  
    char *c;
    float scale = 0.0002f;
    glPushMatrix();
    glTranslatef( x, y, z );
    glScalef( scale, scale, scale );
    for (c=string; *c != '\0'; c++) 
    {
        glutStrokeCharacter(font, *c);
    }
    glPopMatrix();
}

void renderBitmapString( float x, float y, float z, void *font, const char* const string) 
{  
    const char* c;
    glPushMatrix();
    glRasterPos3f(x, y, z );
    for ( c=string; *c != '\0'; c++) 
    {
        glutBitmapCharacter(font, *c);
    }
    glPopMatrix();
}

void display() 
{
    // Clear the screen
    glClearColor(0.0, 0.0, 0.8, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the drawing color (RGB: WHITE)
    glColor3f(1.0,1.0,1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	// Calculate The Aspect Ratio Of The Window
	gluPerspective( s_cameraFoV, (GLfloat)winWidth/ (GLfloat)winHeight, 0.1f, 100.0f );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef( s_cameraX, s_cameraY, s_cameraZ );

	renderSortDraw();

    // Set the viewport to be the entire window
    glViewport(0, 0, winWidth, winHeight);

    glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0);   // Many ways to do this
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	if ( drawStrokeFont == 1 )
	{
    	renderStrokeFontString( 0.2f, 0.5f, 0.0f, strokeFont, "Stroke Font" );
	}

    glViewport(0, 0, winWidth, winHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 120, 0, 20);
    glMatrixMode(GL_MODELVIEW);

    LJ_debugVarRender();

    glFlush(); // Flush the buffer
    SDL_GL_SwapBuffers();
}

void tick() 
{
	struct timespec rqtp;
	struct timespec rmtp;
	renderSortTest();
	display();
#define MILLISECONDS (1000*1000)
	rqtp.tv_sec = 0;
	rqtp.tv_nsec = s_sleepTime * MILLISECONDS;
#undef MILLISECONDS
	nanosleep( &rqtp, &rmtp );
}


void init() 
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, winWidth, winHeight);

    glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0);   // Many ways to do this
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLineWidth(1.0);
    glEnable(GL_DEPTH_TEST);

	s_quadratic = gluNewQuadric();			
	gluQuadricNormals( s_quadratic, GLU_SMOOTH );
	gluQuadricTexture( s_quadratic, GL_TRUE );

}

void specialKeyPressed(int key, int x, int y)
{
	const int modKey = glutGetModifiers(); 
	int fastKey = 0;
	if ( modKey == GLUT_ACTIVE_CTRL )
	{
		fastKey = LJ_DEBUG_VAR_INPUT_FAST;
	}
	switch ( key )
   	{
		default:
		{
			printf( "Unhandled key %d 0x%X\n", key, key );
			break;
		}
	}
}

void keyboard( SDL_KeyboardEvent* const keyEvent )
{
	const SDLMod modifier = keyEvent->keysym.mod;
	int fastKey = 0;
	if ( ( modifier & KMOD_LCTRL ) || ( modifier & KMOD_RCTRL ) )
	{
		fastKey = LJ_DEBUG_VAR_INPUT_FAST;
	}
    switch ( keyEvent->keysym.sym ) 
	{
        case KEY_ESCAPE:  // The escape key
        case 'Q':
        case 'q':
		{
            exit(0);   // Simply exit
            break;
		}
        case 'p':
        case 'P':
		{
            break;
		}
        case 'r':
		{
            LJ_debugVarRender();
            break;
		}
        case 'w':
		{
            LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_UP | fastKey );
            LJ_debugVarRender();
            break;
		}
        case 's':
		{
            LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_DOWN | fastKey );
            LJ_debugVarRender();
            break;
		}
        case 'a':
		{
            LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_LEFT | fastKey );
            LJ_debugVarRender();
            break;
		}
        case 'd':
		{
            LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_RIGHT | fastKey );
            LJ_debugVarRender();
            break;
		}
        case ' ':
		{
            LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_SELECT | fastKey );
            LJ_debugVarRender();
            break;
		}
		case KEY_BACKSPACE:
		{
            LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_CANCEL | fastKey );
            LJ_debugVarRender();
            break;
		}
		case KEY_UP:
		{
            LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_UP | fastKey );
            LJ_debugVarRender();
   			break;
		}
		case KEY_DOWN:
		{
            LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_DOWN | fastKey );
            LJ_debugVarRender();
   			break;
		}
		case KEY_LEFT:
		{
            LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_LEFT | fastKey );
            LJ_debugVarRender();
   			break;
		}
		case KEY_RIGHT:
		{
            LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_RIGHT | fastKey );
            LJ_debugVarRender();
			break;
		}
		default:
		{	
			int symKey = keyEvent->keysym.sym;
			printf( "Unhandled key %d 0x%X '%c' '%s'\n", symKey, symKey, symKey, SDL_GetKeyName( symKey ) );
			break;
		}
    }
}

void WindowActive( void )
{
}

void WindowInactive( void )
{
}
 
void KeyUp( SDL_KeyboardEvent* const keyEvent )
{
}
 
void KeyDown( SDL_KeyboardEvent* const keyEvent )
{
	keyboard( keyEvent );
}
 
void MouseMoved( const int x, const int y, const int relX, const int relY )
{
}
 
void MouseButtonUp( const int button, const int x, const int y, const int relX, const int relY )
{
}

void MouseButtonDown( const int button, const int x, const int y, const int relX, const int relY )
{
}
 
void HandleInput( void )
{
	// Poll for events, and handle the ones we care about.
	SDL_Event event;
	while ( SDL_PollEvent( &event ) ) 
	{
		switch ( event.type ) 
		{
			case SDL_KEYDOWN:
			{
				// If escape is pressed set the Quit-flag
				if ( event.key.keysym.sym == SDLK_ESCAPE )
				{
					s_quit = 1;
					break;
				}
 
				KeyDown( &event.key );
				break;
			}
 
			case SDL_KEYUP:
			{
				KeyUp( &event.key );
				break;
			}
 
			case SDL_QUIT:
			{
				s_quit = 1;
				break;
			}
 
			case SDL_MOUSEMOTION:
			{
				MouseMoved(
							event.motion.x, 
							event.motion.y, 
							event.motion.xrel, 
							event.motion.yrel );
				break;
			}
 
			case SDL_MOUSEBUTTONUP:
			{
				MouseButtonUp(
						event.button.button, 
						event.motion.x, 
						event.motion.y, 
						event.motion.xrel, 
						event.motion.yrel);
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			{
				MouseButtonDown(
						event.button.button, 
						event.motion.x, 
						event.motion.y, 
						event.motion.xrel, 
						event.motion.yrel);
				break;
			}

			case SDL_ACTIVEEVENT:
			{
				if ( event.active.state & SDL_APPACTIVE ) 
				{
					if ( event.active.gain ) 
					{
						s_minimized = 0;
						WindowActive();
					} 
					else 
					{
						s_minimized = 1;
						WindowInactive();
					}
				}
				break;
			}
		}
	} 
}

void mainLoop()
{
//	long lastTick = SDL_GetTicks();
 
	// Main loop: loop forever.
	while ( !s_quit )
	{
		// Handle mouse and keyboard input
		HandleInput();
 
//		if ( s_minimized ) 
//		{
//			// Release some system resources if the app. is minimized.
//			WaitMessage(); // pause the application until focus in regained
//		} 
//		else 
		{
			// tick
			tick();
		}
	}
}


int main(int argc, char* argv[])
{
 	SDL_Surface *screen;

    LJ_debugVarInit( 64 );
    LJ_debugVarRegister( "Camera:x", LJ_DEBUG_VAR_FLOAT, &s_cameraX, 0 );
    LJ_debugVarRegister( "Camera:y", LJ_DEBUG_VAR_FLOAT, &s_cameraY, 0 );
    LJ_debugVarRegister( "Camera:z", LJ_DEBUG_VAR_FLOAT, &s_cameraZ, 0 );
    LJ_debugVarRegister( "Camera:fov", LJ_DEBUG_VAR_FLOAT, &s_cameraFoV, 0 );
    LJ_debugVarRegister( "Main:sleepTime", LJ_DEBUG_VAR_INT, &s_sleepTime, 0 );

	renderSortInit();

	// Initialize SDL
	SDL_Init( SDL_INIT_EVERYTHING );
 
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    // Initialize the GLUT environment - needed for font rendering
    glutInit(&argc, argv);

	screen = SDL_SetVideoMode( winWidth, winHeight, 32, SDL_OPENGL );

    // Initialize OpenGL features
    init();

    //   Enters event-driven (infinite) loop
	mainLoop();

	SDL_Quit();
    LJ_debugVarShutdown();

    return 0;
}

#define MAX_NUM_OUTPUT 8

void sortFunc( float* const testData, const int numData, float* const sortedData )
{
	int i;
	int numOutput = 0;
	for (  i = 0; i < numData; i++ )
	{
		const float score = testData[i];
		if ( 1 || ( numOutput < MAX_NUM_OUTPUT ) )
		{
			int j;
			int min = 0;
			int max = numOutput-1;

			if ( numOutput == 0 )
			{
				sortedData[min] = score;
				numOutput++;
				continue;
			}

			// Find the insertion point
			while ( min != max )
			{
				const int mid = ( min + max ) >> 1;
				if ( ( max - min ) == 1 )
				{
					if ( score > sortedData[mid] )
					{
						min = max;
					}
					break;
				}

				if ( score > sortedData[mid] )
				{
					min = mid;
				}
				else
				{
					max = mid;
				}
			}
			// Need to insert the data at min (= max) index
			// shuffle the items to the right to make space
			for ( j = numOutput; j > min; j-- )
			{
				if ( j < MAX_NUM_OUTPUT )
				{
					sortedData[j] = sortedData[j-1];
				}
			}
			// Insert the data
			sortedData[min] = score;
			if ( numOutput < MAX_NUM_OUTPUT )
			{
				numOutput++;
			}
		}
	}

	{
		float maxScore = -1.0f;
		// Check it is sorted
		for (  i = 0; i < numOutput; i++ )
		{
			const float score = sortedData[i];
			//printf( "Score[%d] %f\n", i, score );
			if ( score < maxScore )
			{
				int j;
				printf( "It is broken %f %f\n", score, maxScore );
				for (  j = 0; j < numOutput; j++ )
				{
					printf( "Score[%d] %f\n", j, sortedData[j] );
				}
				for (  j = 0; j < numData; j++ )
				{
					printf( "Input Score[%d] %f\n", j, testData[j] );
				}
				sortFunc( testData, numData, sortedData );
			}
			maxScore = score;
		}
	}
}

void debugDrawSphere( float x, float y, float z, float size, int colour )
{
	const float red = (float)(( colour >> 24 ) & 0xFF) / 255.0f;
	const float green = (float)(( colour >> 16 ) & 0xFF) / 255.0f;
	const float blue = (float)(( colour >> 8 ) & 0xFF) / 255.0f;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glColor4f( red, green, blue, 0.5f );
	glTranslatef( x, y, z );
	gluSphere( s_quadratic, size, 32, 32 );
    glPopMatrix();
}
