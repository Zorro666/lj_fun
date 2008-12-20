#include "LJ_engine.h"
#include "LJ_DebugVar/LJ_debugVar.h"

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <time.h>

int winWidth = 768;
int winHeight = 512;

SDL_Surface* s_sdlScreen;
GLUquadricObj* s_quadratic;

extern int s_sleepTime;

extern float s_cameraFoV;
extern float s_cameraX;
extern float s_cameraY;
extern float s_cameraZ;

extern int s_quit;
extern int s_minimized;

extern void game3DRender( void );
extern void game2DRender( void );

void renderBitmapString( float x, float y, float z, void *font, const char* const string );
void inputTick( void );

void LJ_engineInit( int argc, char* argv[] )
{
	// Initialize SDL
	SDL_Init( SDL_INIT_EVERYTHING );
 
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    // Initialize the GLUT environment - needed for font rendering
    glutInit(&argc, argv);

	s_sdlScreen = SDL_SetVideoMode( winWidth, winHeight, 32, SDL_OPENGL );

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

//	long lastTick = SDL_GetTicks();

void LJ_engineReset( void )
{
}

void LJ_engineShutdown( void )
{
	SDL_Quit();
}

void LJ_engineTick( void )
{
	struct timespec rqtp;
	struct timespec rmtp;
#define MILLISECONDS (1000*1000)
	rqtp.tv_sec = 0;
	rqtp.tv_nsec = s_sleepTime * MILLISECONDS;
#undef MILLISECONDS
	nanosleep( &rqtp, &rmtp );

	// Handle mouse and keyboard input
	inputTick();
}

void LJ_engineStartRendering( void )
{
    // Clear the screen
    glClearColor(0.0, 0.0, 0.8, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void LJ_engineFinishRendering( void )
{
    glFlush(); // Flush the buffer
    SDL_GL_SwapBuffers();
}

void LJ_engineRender( void )
{
    // Set the drawing color (RGB: WHITE)
    glColor3f(1.0,1.0,1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	// Calculate The Aspect Ratio Of The Window
	gluPerspective( s_cameraFoV, (GLfloat)winWidth/ (GLfloat)winHeight, 0.1f, 100.0f );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef( s_cameraX, s_cameraY, s_cameraZ );

	game3DRender();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, winWidth, winHeight);

    glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	game2DRender();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 120, 0, 20);
    glMatrixMode(GL_MODELVIEW);

    LJ_debugVarRender();
}

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

void LJ_debugDrawSphere( float x, float y, float z, float radius, int colour )
{
	const float red = (float)(( colour >> 24 ) & 0xFF) / 255.0f;
	const float green = (float)(( colour >> 16 ) & 0xFF) / 255.0f;
	const float blue = (float)(( colour >> 8 ) & 0xFF) / 255.0f;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glColor4f( red, green, blue, 0.5f );
	glTranslatef( x, y, z );
	gluSphere( s_quadratic, radius, 32, 32 );
    glPopMatrix();
}

void LJ_debugDrawCircle( float x, float y, float z, float radius, int colour )
{
	const float red = (float)(( colour >> 24 ) & 0xFF) / 255.0f;
	const float green = (float)(( colour >> 16 ) & 0xFF) / 255.0f;
	const float blue = (float)(( colour >> 8 ) & 0xFF) / 255.0f;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glColor4f( red, green, blue, 0.5f );
	glTranslatef( x, y, z );
	gluDisk( s_quadratic, radius-0.01f, radius, 32, 32 );
    glPopMatrix();
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
        case LJ_KEY_ESCAPE:  // The escape key
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
		case LJ_KEY_BACKSPACE:
		{
            LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_CANCEL | fastKey );
            LJ_debugVarRender();
            break;
		}
		case LJ_KEY_UP:
		{
            LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_UP | fastKey );
            LJ_debugVarRender();
   			break;
		}
		case LJ_KEY_DOWN:
		{
            LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_DOWN | fastKey );
            LJ_debugVarRender();
   			break;
		}
		case LJ_KEY_LEFT:
		{
            LJ_debugVarInput( LJ_DEBUG_VAR_INPUT_LEFT | fastKey );
            LJ_debugVarRender();
   			break;
		}
		case LJ_KEY_RIGHT:
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
 
void inputTick( void )
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

