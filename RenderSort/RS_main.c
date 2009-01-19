// main.c : defines the entry point for the console application.

#include "LJ.h"

#include "LJ_LibCommon/LJ_LibCommon.h"
#include "LJ_LibDebug/LJ_LibDebug.h"
#include "LJ_LibEngine/LJ_LibEngine.h"

#include "RS_renderSort.h"

LJ_float s_cameraFoV = 60.0f;
LJ_float s_cameraX = 0.0f;
LJ_float s_cameraY = 0.0f;
LJ_float s_cameraZ = -10.0f;
LJ_int s_sleepTime = 100;

LJ_int s_quit = 0;

#include <GL/glut.h>

void* strokeFont = GLUT_STROKE_MONO_ROMAN;
int drawStrokeFont = 0;

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
			//LJ_outputPrintDebug( ( "Score[%d] %f\n", i, score ) );
			if ( score < maxScore )
			{
				int j;
				LJ_outputPrintGold( ( "It is broken %f %f\n", score, maxScore ) );
				for (  j = 0; j < numOutput; j++ )
				{
					LJ_outputPrintGold( ( "Score[%d] %f\n", j, sortedData[j] ) );
				}
				for (  j = 0; j < numData; j++ )
				{
					LJ_outputPrintGold( ( "Input Score[%d] %f\n", j, testData[j] ) );
				}
				sortFunc( testData, numData, sortedData );
			}
			maxScore = score;
		}
	}
}

void gameTick( void )
{
    renderSortTest();
}

void game3DRender( void )
{
    // Set the drawing color (RGB: WHITE)
    glColor3f(1.0,1.0,1.0);

    renderSortDraw();
}

void game2DRender( void )
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if ( drawStrokeFont == 1 )
    {
    	renderStrokeFontString( 0.2f, 0.5f, 0.0f, strokeFont, "Stroke Font" );
    }
}

void gameSingleLoop()
{
	LJ_engineTick();
	gameTick();
	LJ_engineStartRendering();
	LJ_engineRender();
	LJ_engineFinishRendering();
}

void gameInit( void )
{
    LJ_debugVarRegister( "Camera:x", LJ_DEBUG_VAR_FLOAT, &s_cameraX, 0 );
    LJ_debugVarRegister( "Camera:y", LJ_DEBUG_VAR_FLOAT, &s_cameraY, 0 );
    LJ_debugVarRegister( "Camera:z", LJ_DEBUG_VAR_FLOAT, &s_cameraZ, 0 );
    LJ_debugVarRegister( "Camera:fov", LJ_DEBUG_VAR_FLOAT, &s_cameraFoV, 0 );
    LJ_debugVarRegister( "Main:sleepTime", LJ_DEBUG_VAR_INT, &s_sleepTime, 0 );

    renderSortInit();
}

void gameReset( void )
{
}

void gameShutdown( void )
{
}

void debugDrawSphere( float x, float y, float z, float size, int colour )
{
    LJ_debugDrawSphere( x, y, z, size, colour );
}

int main(int argc, char* argv[])
{
    LJ_engineEarlyInit( argc, argv );
	gameInit();
	LJ_engineLateInit();

	while ( !s_quit )
	{
	    gameSingleLoop();
	}

	gameShutdown();
	LJ_engineShutdown();

    return 0;
}


