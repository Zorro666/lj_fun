// debugVar.cpp 
//
#include "debugVar.h"

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

typedef struct debugVarDef_s
{
    const char* name;
    unsigned int flags;
    void* dataPtr;
} debugVarDef;

// Change 1

typedef struct debugVarValue_s
{
	union 
	{
		int iVal;
		float fVal;
	};
} debugVarValue;

#define DEBUG_VAR_MAX_NAME_LENGTH (256)
#define DEBUG_VAR_MAX_DEPTH (16)
#define DEBUG_VAR_DELIMETER_CHAR (':')

//
// Private data
//

debugVarDef* g_debugVarArray = NULL;
float* g_debugVarMenuScrollStart;
int g_debugVarNum = 0;
int g_debugVarMaxNum = 0;

debugVarDef* g_selectedItem = NULL;
int g_selectedDepth = 0;

int g_editingMode = 0;
debugVarValue g_variableSavedValue;
int g_intVariableAlterAmount = 1;
float g_floatVariableAlterAmount = 0.1f;

const int g_menuMaxY = 8;
float g_menuSelectedPlace = 0.0f;
//
// Private functions
//

void debugVarGetValue( const debugVarDef* const var, debugVarValue* const value )
{
	if ( var->flags & DEBUG_VAR_BOOL )
	{
		memcpy( &value->iVal, var->dataPtr, sizeof( int ) );
	}
	else if ( var->flags & DEBUG_VAR_INT )
	{
		memcpy( &value->iVal, var->dataPtr, sizeof( int ) );
	}
	else if ( var->flags & DEBUG_VAR_FLOAT )
	{
		memcpy( &value->fVal, var->dataPtr, sizeof( float ) );
	}
}

void debugVarSetValue( const debugVarValue* const value, debugVarDef* const var )
{
	if ( var->flags & DEBUG_VAR_BOOL )
	{
		memcpy( var->dataPtr, &value->iVal, sizeof( int ) );
	}
	else if ( var->flags & DEBUG_VAR_INT )
	{
		memcpy( var->dataPtr, &value->iVal, sizeof( int ) );
	}
	else if ( var->flags & DEBUG_VAR_FLOAT )
	{
		memcpy( var->dataPtr, &value->fVal, sizeof( float ) );
	}
}

void debugVarPrintValue( const int flags, const debugVarValue* const value, char* const output )
{
	if ( flags & DEBUG_VAR_BOOL )
	{
		sprintf( output, "%s", ( value->iVal == 1 ) ? "TRUE" : "FALSE" );
	}
	else if ( flags & DEBUG_VAR_INT )
	{
		sprintf( output, "%d", value->iVal );
	}
	else if ( flags & DEBUG_VAR_FLOAT )
	{
		sprintf( output, "%f", value->fVal );
	}
}

void debugVarAlterIncValue( const int increment )
{
	if ( g_selectedItem->flags & DEBUG_VAR_INT )
	{
		if ( g_intVariableAlterAmount >= 500 )
		{
			g_intVariableAlterAmount += increment * 100;
		}
		else if ( g_intVariableAlterAmount >= 100 )
		{
			g_intVariableAlterAmount += increment * 50;
		}
		else if ( g_intVariableAlterAmount >= 10 )
		{
			g_intVariableAlterAmount += increment * 5;
		}
		else
		{
			g_intVariableAlterAmount += increment * 1;
		}
		if ( g_intVariableAlterAmount < 1 )
		{
			g_intVariableAlterAmount = 1;
		}
		else if ( g_intVariableAlterAmount > 1000 )
		{
			g_intVariableAlterAmount = 1000;
		}
	}
	else if ( g_selectedItem->flags & DEBUG_VAR_FLOAT )
	{
		if ( g_floatVariableAlterAmount >= 500.0f )
		{
			g_floatVariableAlterAmount += 100.0f * (float)increment;
		}
		else if ( g_floatVariableAlterAmount >= 100.0f )
		{
			g_floatVariableAlterAmount += 50.0f * (float)increment;
		}
		else if ( g_floatVariableAlterAmount >= 10.0f )
		{
			g_floatVariableAlterAmount += 5.0f * (float)increment;
		}
		else if ( g_floatVariableAlterAmount >= 2.0f )
		{
			g_floatVariableAlterAmount += 1.0f * (float)increment;
		}
		else 
		{
			g_floatVariableAlterAmount += 0.1f * (float)increment;
		}
		if ( g_floatVariableAlterAmount < 0.1f )
		{
			g_floatVariableAlterAmount = 0.1f;
		}
		else if ( g_floatVariableAlterAmount > 1000.0f )
		{
			g_floatVariableAlterAmount = 1000.0f;
		}
	}
}

void debugVarStartEditMode()
{
	char output[DEBUG_VAR_MAX_NAME_LENGTH];
	g_editingMode = 1;
	debugVarGetValue( g_selectedItem, &g_variableSavedValue );
	debugVarPrintValue( g_selectedItem->flags, &g_variableSavedValue, output );
	printf( "StartEditMode old Value %s\n", output );
}

void debugVarEndEditMode( const int cancel )
{
	char output[DEBUG_VAR_MAX_NAME_LENGTH];
	debugVarValue tempValue;
	g_editingMode = 0;
	if ( cancel == 1 )
	{
		debugVarSetValue( &g_variableSavedValue, g_selectedItem );
		printf( "cancel\n" );
	}
	debugVarGetValue( g_selectedItem, &tempValue );
	debugVarPrintValue( g_selectedItem->flags, &tempValue, output );
	printf( "EndEditMode new Value %s\n", output );
}

// increment can be +1, -1, +10, -10 e.g. normal, fast
void debugVarAlterValue( const int increment, debugVarDef* const var )
{
	debugVarValue variableValue;
	debugVarGetValue( var, &variableValue );
	if ( var->flags & DEBUG_VAR_BOOL )
	{
		// For bool's just change true<->false
		variableValue.iVal ^= 1;
	}
	else if ( var->flags & DEBUG_VAR_INT )
	{
		variableValue.iVal += increment * g_intVariableAlterAmount;
	}
	else if ( var->flags & DEBUG_VAR_FLOAT )
	{
		variableValue.fVal += (float)increment * g_floatVariableAlterAmount;
	}
	debugVarSetValue( &variableValue, var );
}

// number of : in the name
int debugVarCountDepth( const char* const name )
{
	const char* delimPtr = name;
	int count = 0;
    while ( delimPtr != NULL )
	{
        delimPtr = strchr( delimPtr, DEBUG_VAR_DELIMETER_CHAR );
		if ( delimPtr != NULL )
		{
			count++;
			delimPtr++;
		}
	}
	return count;
}

int debugVarGetScope( const char* const name, const int depth, char* const scope, const int scopeMaxLength, const int fullScope )
{
    int scopeStart = 0;
    const char* scopeString = name + scopeStart;
    int delimCount = 0;
    int index = 0;

    if ( depth < 0 )
    {
        scope[0] = '\0';
        return 0;
    }
    // Try to find the n'th delimeter in this string
    while ( delimCount < depth )
    {
        const char* delimPtr = strchr( scopeString, DEBUG_VAR_DELIMETER_CHAR );
        if ( delimPtr != NULL )
        {
            scopeString = delimPtr + 1;
			delimCount++;
        }
        else
        {
            return 0;
        }
    }
	if ( fullScope == 1 )
	{
		scopeString = name;
		delimCount = 0;
	}
	else
	{
		delimCount = depth + 1;
	}
    // If the delimeter isn't found then the scope if the start->end of the string
    // else copy until start == end ptr
    while ( index < (scopeMaxLength-1) )
    {
        const char inputChar = scopeString[index];
        // Stop if we find the end of the string
        if ( inputChar == '\0' )
        {
            break;
        }
        // Stop if find the n'th delimeter
        if ( inputChar == DEBUG_VAR_DELIMETER_CHAR )
        {
			if ( fullScope == 1 )
			{
				delimCount++;
			}
			if ( delimCount > depth )
			{
            	break;
			}
        }
        scope[index] = inputChar;
        index++;
    }
    scope[index] = '\0';
    return 1;
}

#define DEBUGVAR_SORT_DEBUG 0
// Sort the debug var array by name
void debugVarSort( void )
{
    // Very basic bubble sort
	int i;
	int j;
    const int numVars = g_debugVarNum;
#if DEBUGVAR_SORT_DEBUG
	printf( "---------------------------------------\n" );
	printf( "debugVarSort %d\n", numVars );
    for ( i = 0; i < numVars; i++ )
    {
		debugVarDef* const debugVarI = &g_debugVarArray[i];
		printf( "Var[%d/%d] '%s'\n", i, numVars, debugVarI->name );
	}
	printf( "---------------------------------------\n" );
#endif // #if DEBUGVAR_SORT_DEBUG
    for ( i = 0; i < ( numVars - 1 ); i++ )
    {
        for ( j = ( i + 1 ); j < numVars; j++ )
        {
            debugVarDef* const debugVarI = &g_debugVarArray[i];
            debugVarDef* const debugVarJ = &g_debugVarArray[j];

			// Don't do ordinary alpha sort
			// Compare the scopes one at a time and sort by the scope alpha value
			// put items with no matching scope below items with a scope
			const int depthI = debugVarCountDepth( debugVarI->name );
			const int depthJ = debugVarCountDepth( debugVarJ->name );
			const int minDepth = ( depthI < depthJ ) ? depthI : depthJ;

			int swap = 0;
			int d;
#if DEBUGVAR_SORT_DEBUG
			printf( "---------------------------------------\n" );
			printf( "DepthI %d Var '%s'\n", depthI, debugVarI->name );
			printf( "DepthJ %d Var '%s'\n", depthJ, debugVarJ->name );
			printf( "minDepth %d\n", minDepth );
#endif // #if DEBUGVAR_SORT_DEBUG
			for ( d = 0; d <= minDepth; d++ )
			{
				int scopeMatch = 0;
				char scopeI[DEBUG_VAR_MAX_NAME_LENGTH];
				char scopeJ[DEBUG_VAR_MAX_NAME_LENGTH];
				debugVarGetScope( debugVarI->name, d, scopeI, DEBUG_VAR_MAX_NAME_LENGTH, 0 );
				debugVarGetScope( debugVarJ->name, d, scopeJ, DEBUG_VAR_MAX_NAME_LENGTH, 0 );
				scopeMatch = strcmp( scopeI, scopeJ );
#if DEBUGVAR_SORT_DEBUG
				printf( "I Var[%d/%d] Scope[%d/%d] '%s'\n", i, numVars, d, depthI, scopeI );
				printf( "J Var[%d/%d] Scope[%d/%d] '%s'\n", j, numVars, d, depthJ, scopeJ );
				printf( "ScopeMatch %d\n", scopeMatch );
#endif // #if DEBUGVAR_SORT_DEBUG
            	if ( scopeMatch > 0 )
            	{
					swap = 1;
#if DEBUGVAR_SORT_DEBUG
					printf( "1 Swap 1\n" );
#endif // #if DEBUGVAR_SORT_DEBUG
					// Don't use this result if I has more depth than J and we at the end of J's Depth
					if ( ( depthI > depthJ ) && ( d == depthJ ) )
					{
						swap = 0;
#if DEBUGVAR_SORT_DEBUG
						printf( "1 Swap 0\n" );
#endif // #if DEBUGVAR_SORT_DEBUG
						d = minDepth+1;
						continue;
					}
					if ( depthI >= depthJ )
					{
						d = minDepth+1;
						continue;
					}
				}
				else if ( scopeMatch < 0 )
				{
					// Don't use this result if I has more depth than J and we at the end of J's Depth
					if ( ( depthI < depthJ ) && ( d == depthI ) )
					{
						swap = 1;
#if DEBUGVAR_SORT_DEBUG
						printf( "2 Swap 1\n" );
#endif // #if DEBUGVAR_SORT_DEBUG
						d = minDepth+1;
						continue;
					}
					//if ( depthI >= depthJ )
					if ( ( depthI > 0 ) && ( depthJ > 0 ) )
					{
						d = minDepth+1;
						continue;
					}
				}
			}
			if ( swap == 1 )
			{
              	const debugVarDef temp = *debugVarI;
               	*debugVarI = *debugVarJ;
               	*debugVarJ = temp;
#if DEBUGVAR_SORT_DEBUG
				printf( "Do the swap\n" );
#endif // #if DEBUGVAR_SORT_DEBUG
           	}
        }
    }
#if DEBUGVAR_SORT_DEBUG
	printf( "---------------------------------------\n" );
    for ( i = 0; i < numVars; i++ )
    {
		debugVarDef* const debugVarI = &g_debugVarArray[i];
		printf( "Var[%d/%d] '%s'\n", i, numVars, debugVarI->name );
	}
	printf( "***************************************\n" );
#endif // #if DEBUGVAR_SORT_DEBUG
}

debugVarDef* debugVarFindVar( const debugVarDef* const currentVar, 
							  const char* const parentScope, const char* const currentScope, 
							  const int depth, const int direction )
{
	const int startIndex = currentVar - g_debugVarArray;
	const int numVars = g_debugVarNum;
	const int endIndex = ( direction > 0 ) ? numVars : -1; 
	debugVarDef* newScopeVar = NULL;
	char testScope[DEBUG_VAR_MAX_NAME_LENGTH];
	int i;
	debugVarDef* lastVar = NULL;

	strncpy( testScope, currentScope, DEBUG_VAR_MAX_NAME_LENGTH );

	for ( i = startIndex; i != endIndex; i+= direction )
	{
		char varParentScope[DEBUG_VAR_MAX_NAME_LENGTH];
		debugVarDef* const debugVar = &g_debugVarArray[i];
		// Get the parent scope of this variable
		debugVarGetScope( debugVar->name, (depth-1), varParentScope, DEBUG_VAR_MAX_NAME_LENGTH, 1 );
        // Only consider things with the same parent scope
        if ( strcmp( parentScope, varParentScope ) == 0 )
        {
            char varScope[DEBUG_VAR_MAX_NAME_LENGTH];
			// Get the current scope of this variable
            if ( debugVarGetScope( debugVar->name, depth, varScope, DEBUG_VAR_MAX_NAME_LENGTH, 0 ) == 1 )
            {
				// Only consider things with a different current scope
            	if ( strcmp( testScope, varScope ) != 0 )
               	{
					// If going down in the list need to find the first occurance of the new scope
					if ( direction > 0 )
					{
						newScopeVar = debugVar;
						lastVar = newScopeVar;
					}
					// If going up in the list need to find the last occurance of the new scope
					if ( newScopeVar == NULL )
					{
						newScopeVar = debugVar;
						lastVar = newScopeVar;
						strncpy( testScope, varScope, DEBUG_VAR_MAX_NAME_LENGTH );
					}
					else
					{
						return lastVar;
					}
				}
				lastVar = debugVar;
			}
		}
	}
	return lastVar;
}

debugVarDef* debugVarFindVarAtScope( const debugVarDef* const currentVar, const int depth, const int direction )
{
    char parentScopeString[DEBUG_VAR_MAX_NAME_LENGTH];
    char currentScopeString[DEBUG_VAR_MAX_NAME_LENGTH];

    debugVarGetScope( currentVar->name, (depth-1), parentScopeString, DEBUG_VAR_MAX_NAME_LENGTH, 1 );
    //printf( "parentScopeString : %s\n", parentScopeString );
    debugVarGetScope( currentVar->name, depth, currentScopeString, DEBUG_VAR_MAX_NAME_LENGTH, 0 );
    //printf( "currentScopeString : %s\n", currentScopeString );
	return debugVarFindVar( currentVar, parentScopeString, currentScopeString, depth, direction );
}

// Public functions
void debugVarInitialise( const int maxNumVars )
{
	g_debugVarMenuScrollStart = NULL;
	g_debugVarArray = NULL;
	g_debugVarNum = 0;
	g_debugVarMaxNum = maxNumVars;

	if ( maxNumVars > 0 )
	{
		g_debugVarArray = (debugVarDef*)debugVarMemAlloc( sizeof( debugVarDef ) * maxNumVars );
		g_debugVarMenuScrollStart = (float*)debugVarMemAlloc( sizeof( float ) * DEBUG_VAR_MAX_DEPTH );
	}

	debugVarReset();
}

void debugVarReset( void )
{
	g_selectedDepth = 0;
	g_editingMode = 0;
	g_selectedItem = g_debugVarArray;
	if ( g_debugVarMenuScrollStart != NULL )
	{
		int d;
		for ( d = 0; d < DEBUG_VAR_MAX_DEPTH; d++ )
		{
			g_debugVarMenuScrollStart[d] = 0.0f;
		}
	}
	g_intVariableAlterAmount = 1;
	g_floatVariableAlterAmount = 0.1f;
	g_menuSelectedPlace = 0.0f;
}

void debugVarDeInitialise( void )
{
	debugVarReset();
	g_debugVarNum = 0;
	debugVarMemFree( g_debugVarArray );
	debugVarMemFree( g_debugVarMenuScrollStart );
}

int debugVarRegister( const char* const name, const int type, void* dataPtr, const int flags )
{
    debugVarDef* const debugVar = &g_debugVarArray[g_debugVarNum];
    debugVar->name = name;
    debugVar->dataPtr = dataPtr;
    debugVar->flags = type | flags;
    g_debugVarNum++;

    debugVarSort();
    debugVarSort();
    return 1;
}

void debugVarRender( void )
{
    int depth = 0;
	char tempString[DEBUG_VAR_MAX_NAME_LENGTH];
	char currentScope[DEBUG_VAR_MAX_NAME_LENGTH];
    const float xOrigin = 0.4f;
	const float yOrigin = 0.2f;
	float x0 = xOrigin;
	float y0 = yOrigin;
	float y1 = y0;
	float yStart = y0;
	float maxX = 0.0f;
	const unsigned int bgColour = 0x000000FF; 
	const unsigned int infoTextColour = 0xFFFFFFFF; 
	const int maxDepth = debugVarCountDepth( g_selectedItem->name );
#if DEBUGVAR_RENDER_DEBUG
    printf( "-------------------------------\n" );
    printf( "SelectedIndex: %d SelectedDepth %d/%d\n", ( g_selectedItem - g_debugVarArray ), g_selectedDepth, maxDepth );
#endif // #if DEBUGVAR_RENDER_DEBUG

	float xLen = 0.0f;
	tempString[0] = '\0';
	// Nice BG box to put info text in
	y1 += 1.5f;
	if ( g_editingMode == 1 )
	{
		// Get the length
		if ( g_selectedItem->flags & DEBUG_VAR_BOOL )
		{
			sprintf( tempString, "Edit Mode: BOOL value" );
		}
		else if ( g_selectedItem->flags & DEBUG_VAR_INT )
		{
			sprintf( tempString, "Edit Mode: SELECT to accept, CANCEL to abort: Inc: %d", g_intVariableAlterAmount );
		}
		else if ( g_selectedItem->flags & DEBUG_VAR_FLOAT )
		{
			sprintf( tempString, "Edit Mode: SELECT to accept, CANCEL to abort: Inc: %f", g_floatVariableAlterAmount );
		}
		xLen = debugVarRenderText( 0, x0, y0, infoTextColour, tempString );
	}
	else
	{
		if ( g_selectedDepth == maxDepth )
		{
			if ( g_selectedItem->flags & DEBUG_VAR_BOOL )
			{
				sprintf( tempString, "Menu Mode : SELECT to toggle BOOL variable" );
			}
			else
			{
				sprintf( tempString, "Menu Mode : Press SELECT to enter edit mode" );
			}
			if ( g_selectedItem->flags & DEBUG_VAR_READ_ONLY )
			{
				sprintf( tempString, "Menu Mode : READ ONLY value     " );
			}
		}
		else
		{
			sprintf( tempString, "Menu Mode    " );
		}
		xLen = debugVarRenderText( 0, x0, y0, infoTextColour, tempString );
	}
	debugVarDrawBackground( x0, y0, x0+xLen, y1, bgColour );
	// Draw the text
	debugVarRenderText( 1, x0+0.9f, y0, infoTextColour, tempString );

	yStart = y1 + 0.2f;

    while ( depth <= g_selectedDepth )
    {
		const float yMenuStart = g_debugVarMenuScrollStart[depth];
		const int numVars = g_debugVarNum;
		int i;
		int p;
    	x0 += maxX;
		maxX = 0.0f;
		y0 = yStart;

		// Two passes 
		// 1st pass work out size and draw background 
		// 2nd pass - draw the text
		for ( p = 0; p < 2; p++ )
		{
			const float yThisMenuStart = yStart;
			float yCount = 0.0f;
			float y = yStart;
			char parentScope[DEBUG_VAR_MAX_NAME_LENGTH];
			char selectedScope[DEBUG_VAR_MAX_NAME_LENGTH];
			currentScope[0] = '\0';

			// Get the parent scope of the selected item
			debugVarGetScope( g_selectedItem->name, (depth-1), parentScope, DEBUG_VAR_MAX_NAME_LENGTH, 1 );
			// Get the current scope of the selected item
			debugVarGetScope( g_selectedItem->name, (depth), selectedScope, DEBUG_VAR_MAX_NAME_LENGTH, 0 );

#if DEBUGVAR_RENDER_DEBUG
			printf( "Parent Scope: %s\n", parentScope );
			printf( "Selected Scope: %s\n", selectedScope );
#endif // #if DEBUGVAR_RENDER_DEBUG
			for ( i = 0; i < numVars; i++ )
			{
				const debugVarDef* const debugVar = &g_debugVarArray[i];
				// Get the parent scope of this variable
				char varParentScope[DEBUG_VAR_MAX_NAME_LENGTH];
				debugVarGetScope( debugVar->name, (depth-1), varParentScope, DEBUG_VAR_MAX_NAME_LENGTH, 1 );

				if ( ( y - yThisMenuStart ) >= g_menuMaxY )
				{
					continue;
				}
				// Ignore things with the wrong parent scope
				if ( strcmp( parentScope, varParentScope ) == 0 )
				{
					char varScope[DEBUG_VAR_MAX_NAME_LENGTH];
					if ( debugVarGetScope( debugVar->name, depth, varScope, DEBUG_VAR_MAX_NAME_LENGTH, 0 ) == 1 )
					{
						if ( strcmp( currentScope, varScope ) != 0 )
						{
							unsigned int colour = 0x909090FF;
							char outputString[256];
							int selected = 0;
							int selected_colour = 0xFFFF00FF;
							const int varMaxDepth = debugVarCountDepth( debugVar->name );
							float endX = 0.0f;

							strncpy( currentScope, varScope, DEBUG_VAR_MAX_NAME_LENGTH );
							if ( strcmp( selectedScope, varScope ) == 0 )
							{
								selected = 1;
								g_menuSelectedPlace = ( y - yStart );
								if ( p == 1 )
								{
									if ( yCount >= yMenuStart )
									{
										yStart = y; 
									}
								}
							}

							if ( ( selected == 1 ) && ( depth < varMaxDepth ) )
							{
								sprintf( outputString, "> %s", varScope );
							}
							else
							{
								sprintf( outputString, "%s", varScope );
							}
							if ( depth == varMaxDepth )
							{
								char outputValue[DEBUG_VAR_MAX_NAME_LENGTH];
								debugVarValue tempValue;
								debugVarGetValue( debugVar, &tempValue );
								debugVarPrintValue( debugVar->flags, &tempValue, outputValue );
								sprintf( outputString, "%s: %s", outputString, outputValue );
								if ( selected == 1 )
								{
									if ( g_editingMode == 1 )
									{
										selected_colour = 0x00FF00FF;
									}
									else
									{
										selected_colour = 0x0000FFFF;
									}
								}
								if ( debugVar->flags & DEBUG_VAR_READ_ONLY )
								{
									colour = 0xFF0000FF;
								}
							}
							if ( selected == 1 )
							{
								colour = selected_colour;
							}
							if ( p == 0 )
							{
								colour = 0x000000FF;
							}
							if ( yCount >= yMenuStart )
							{
								endX = debugVarRenderText( p, x0+0.8f, y, colour, outputString ) + 2.9f;
								if ( endX > maxX )
								{
									maxX = endX;
								}
								y += 1.0f;
							}
							yCount += 1.0f;
						}
					}
				}
			}
			if ( p == 0 )
			{
				const float x1 = x0 + maxX - 0.4f;
				const float y1 = y + 1;
				debugVarDrawBackground( x0, y0, x1, y1, bgColour );
			}
		}
#if DEBUGVAR_DEBUG_MENU_POS
		sprintf( tempString, "start %3.1f", g_debugVarMenuScrollStart[depth] );
		debugVarRenderText( 1, x0, y1+5.0f, 0xFFFFFFFF, tempString );
#endif // #if DEBUGVAR_DEBUG_MENU_POS
        depth++;
    };
#if DEBUGVAR_DEBUG_MENU_POS
	sprintf( tempString, "g_menuSelectedPlace %f", g_menuSelectedPlace );
	debugVarRenderText( 1, 0.0f, 12.0f, 0xFFFFFFFF, tempString );
#endif // #if DEBUGVAR_DEBUG_MENU_POS
}

// Valid inputs are:
// DEBUG_VAR_INPUT_UP
// DEBUG_VAR_INPUT_DOWN
// DEBUG_VAR_INPUT_LEFT
// DEBUG_VAR_INPUT_RIGHT
// DEBUG_VAR_INPUT_SELECT
// DEBUG_VAR_INPUT_CANCEL
void debugVarInput( const int debugInputKey )
{
	const int fastKey = debugInputKey & DEBUG_VAR_INPUT_FAST;
	const int debugKey = debugInputKey & 0xFFFF;
	int newVarDirection = 0;
	int maxDepth = 0;
    int selectedIndex = 0;
	const int incAmt = ( fastKey ) ? 10 : 1;
	const int varMaxDepth = debugVarCountDepth( g_selectedItem->name );
    // Need to make the inputs match the menu display!
    if ( debugKey == DEBUG_VAR_INPUT_UP )
    {
		if ( ( g_selectedDepth == varMaxDepth ) && ( g_editingMode == 1 ) )
		{
			// If in editing mode then increment the value
			debugVarAlterValue( +incAmt, g_selectedItem );
		}
		else
		{
			// Otherwise go up the menu
			newVarDirection = -1;
		}
    }
    if ( debugKey == DEBUG_VAR_INPUT_DOWN )
    {
		if ( ( g_selectedDepth == varMaxDepth ) && ( g_editingMode == 1 ) )
		{
			// If in editing mode then decrement the value
			debugVarAlterValue( -incAmt, g_selectedItem );
		}
		else
		{
			// Otherwise go down the menu
			newVarDirection = +1;
		}
	}
    if ( debugKey == DEBUG_VAR_INPUT_SELECT )
    {
		// If the currently selected item is a variable then SELECT enters/exits edit mode
		if ( g_selectedDepth == varMaxDepth )
		{
			if ( g_editingMode == 1 )
			{
				debugVarEndEditMode( 0 );
			}
			else if ( ( g_selectedItem->flags & DEBUG_VAR_READ_ONLY ) == 0 )
			{
				// Don't enter edit mode for bool vars (just toggle them)
				if ( g_selectedItem->flags & DEBUG_VAR_BOOL )
				{
					debugVarAlterValue( +1, g_selectedItem );
				}
				else
				{
					debugVarStartEditMode();
				}
			}
		}
		else
		{
        	g_selectedDepth++;
		}
    }
    if ( debugKey == DEBUG_VAR_INPUT_CANCEL )
    {
		// If in editing mode then CANCEL stops editing and puts back the old value
		if ( ( g_selectedDepth == varMaxDepth ) && ( g_editingMode == 1 ) )
		{
			debugVarEndEditMode( 1 );
		}
		else
		{
        	g_selectedDepth--;
		}
    }
    if ( debugKey == DEBUG_VAR_INPUT_RIGHT )
    {
		if ( ( g_selectedDepth == varMaxDepth ) && ( g_editingMode == 1 ) )
		{
			// If in editing mode then increase the increment value
			debugVarAlterIncValue( +incAmt );
		}
		else
		{
        	g_selectedDepth++;
		}
    }
    if ( debugKey == DEBUG_VAR_INPUT_LEFT )
    {
		if ( ( g_selectedDepth == varMaxDepth ) && ( g_editingMode == 1 ) )
		{
			debugVarAlterIncValue( -incAmt );
		}
		else
		{
        	g_selectedDepth--;
		}
	}
	if ( newVarDirection != 0 )
	{
		const debugVarDef* const currentVar = g_selectedItem;
		debugVarDef* const newVar = debugVarFindVarAtScope( currentVar, g_selectedDepth, newVarDirection );
		if ( newVar != NULL )
		{
        	g_selectedItem = newVar;
			if ( newVar != currentVar )
			{
				int depth;
				const float newSelectedPlace = g_menuSelectedPlace + newVarDirection;
				if ( newSelectedPlace < 0.0f )
				{
					g_debugVarMenuScrollStart[g_selectedDepth] -= 1.0f;
				}
				else if ( newSelectedPlace >= g_menuMaxY )
				{
					g_debugVarMenuScrollStart[g_selectedDepth] += 1.0f;
				}
				// Reset where the sub-menus start
				for ( depth = g_selectedDepth+1; depth < DEBUG_VAR_MAX_DEPTH; depth++ )
				{
					g_debugVarMenuScrollStart[depth] = 0.0f;
				}
			}
		}
    }
    if ( g_selectedDepth < 0 )
    {
        g_selectedDepth  = 0;
    }
    if ( g_selectedDepth > DEBUG_VAR_MAX_DEPTH )
    {
        g_selectedDepth  = DEBUG_VAR_MAX_DEPTH;
    }
	maxDepth = debugVarCountDepth( g_selectedItem->name );
	if ( g_selectedDepth > maxDepth )
	{
		g_selectedDepth = maxDepth;
	}
    selectedIndex = g_selectedItem - g_debugVarArray;
    if ( selectedIndex < 0 )
    {
        g_selectedItem = &g_debugVarArray[0];
    }
    if ( selectedIndex > ( g_debugVarNum - 1 ) )
    {
        g_selectedItem = &g_debugVarArray[g_debugVarNum-1];
    }
}

