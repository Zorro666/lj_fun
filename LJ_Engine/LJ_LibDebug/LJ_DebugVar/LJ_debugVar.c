// LJ_debugVar.c 
//
#include "LJ_debugVar.h"

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

typedef struct LJ_debugVarDef_s
{
    const LJ_char* name;
    LJ_uint flags;
    void* dataPtr;
} LJ_debugVarDef;

typedef struct LJ_debugVarValue_s
{
	union 
	{
		LJ_int iVal;
		LJ_float fVal;
	};
} LJ_debugVarValue;

#define LJ_DEBUG_VAR_MAX_NAME_LENGTH (256)
#define LJ_DEBUG_VAR_MAX_DEPTH (16)
#define LJ_DEBUG_VAR_DELIMETER_CHAR (':')

//
// Private data
//

LJ_debugVarDef* g_debugVarArray = NULL;
LJ_float* g_debugVarMenuScrollStart;
LJ_int g_debugVarNum = 0;
LJ_int g_debugVarMaxNum = 0;

LJ_debugVarDef* g_selectedItem = NULL;
LJ_int g_selectedDepth = 0;

LJ_int g_editingMode = 0;
LJ_debugVarValue g_variableSavedValue;
LJ_int g_intVariableAlterAmount = 1;
LJ_float g_floatVariableAlterAmount = 0.1f;

const LJ_int g_menuMaxY = 8;
LJ_float g_menuSelectedPlace = 0.0f;
//
// Private functions
//

void LJ_debugVarGetValue( const LJ_debugVarDef* const var, LJ_debugVarValue* const value )
{
	if ( var->flags & LJ_DEBUG_VAR_BOOL )
	{
		memcpy( &value->iVal, var->dataPtr, sizeof( LJ_int ) );
	}
	else if ( var->flags & LJ_DEBUG_VAR_INT )
	{
		memcpy( &value->iVal, var->dataPtr, sizeof( LJ_int ) );
	}
	else if ( var->flags & LJ_DEBUG_VAR_FLOAT )
	{
		memcpy( &value->fVal, var->dataPtr, sizeof( LJ_float ) );
	}
}

void LJ_debugVarSetValue( const LJ_debugVarValue* const value, LJ_debugVarDef* const var )
{
	if ( var->flags & LJ_DEBUG_VAR_BOOL )
	{
		memcpy( var->dataPtr, &value->iVal, sizeof( LJ_int ) );
	}
	else if ( var->flags & LJ_DEBUG_VAR_INT )
	{
		memcpy( var->dataPtr, &value->iVal, sizeof( LJ_int ) );
	}
	else if ( var->flags & LJ_DEBUG_VAR_FLOAT )
	{
		memcpy( var->dataPtr, &value->fVal, sizeof( LJ_float ) );
	}
}

void LJ_debugVarPrintValue( const LJ_int flags, const LJ_debugVarValue* const value, LJ_char* const output )
{
	if ( flags & LJ_DEBUG_VAR_BOOL )
	{
		sprintf( output, "%s", ( value->iVal == 1 ) ? "TRUE" : "FALSE" );
	}
	else if ( flags & LJ_DEBUG_VAR_INT )
	{
		sprintf( output, "%d", value->iVal );
	}
	else if ( flags & LJ_DEBUG_VAR_FLOAT )
	{
		sprintf( output, "%f", value->fVal );
	}
}

void LJ_debugVarAlterIncValue( const LJ_int increment )
{
	if ( g_selectedItem->flags & LJ_DEBUG_VAR_INT )
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
	else if ( g_selectedItem->flags & LJ_DEBUG_VAR_FLOAT )
	{
		if ( g_floatVariableAlterAmount >= 500.0f )
		{
			g_floatVariableAlterAmount += 100.0f * (LJ_float)increment;
		}
		else if ( g_floatVariableAlterAmount >= 100.0f )
		{
			g_floatVariableAlterAmount += 50.0f * (LJ_float)increment;
		}
		else if ( g_floatVariableAlterAmount >= 10.0f )
		{
			g_floatVariableAlterAmount += 5.0f * (LJ_float)increment;
		}
		else if ( g_floatVariableAlterAmount >= 2.0f )
		{
			g_floatVariableAlterAmount += 1.0f * (LJ_float)increment;
		}
		else 
		{
			g_floatVariableAlterAmount += 0.1f * (LJ_float)increment;
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

void LJ_debugVarStartEditMode()
{
	LJ_char output[LJ_DEBUG_VAR_MAX_NAME_LENGTH];
	g_editingMode = 1;
	LJ_debugVarGetValue( g_selectedItem, &g_variableSavedValue );
	LJ_debugVarPrintValue( g_selectedItem->flags, &g_variableSavedValue, output );
	printf( "StartEditMode old Value %s\n", output );
}

void LJ_debugVarEndEditMode( const LJ_int cancel )
{
	LJ_char output[LJ_DEBUG_VAR_MAX_NAME_LENGTH];
	LJ_debugVarValue tempValue;
	g_editingMode = 0;
	if ( cancel == 1 )
	{
		LJ_debugVarSetValue( &g_variableSavedValue, g_selectedItem );
		printf( "cancel\n" );
	}
	LJ_debugVarGetValue( g_selectedItem, &tempValue );
	LJ_debugVarPrintValue( g_selectedItem->flags, &tempValue, output );
	printf( "EndEditMode new Value %s\n", output );
}

// increment can be +1, -1, +10, -10 e.g. normal, fast
void LJ_debugVarAlterValue( const LJ_int increment, LJ_debugVarDef* const var )
{
	LJ_debugVarValue variableValue;
	LJ_debugVarGetValue( var, &variableValue );
	if ( var->flags & LJ_DEBUG_VAR_BOOL )
	{
		// For bool's just change true<->false
		variableValue.iVal ^= 1;
	}
	else if ( var->flags & LJ_DEBUG_VAR_INT )
	{
		variableValue.iVal += increment * g_intVariableAlterAmount;
	}
	else if ( var->flags & LJ_DEBUG_VAR_FLOAT )
	{
		variableValue.fVal += (LJ_float)increment * g_floatVariableAlterAmount;
	}
	LJ_debugVarSetValue( &variableValue, var );
}

// number of : in the name
LJ_int LJ_debugVarCountDepth( const LJ_char* const name )
{
	const LJ_char* delimPtr = name;
	LJ_int count = 0;
    while ( delimPtr != NULL )
	{
        delimPtr = strchr( delimPtr, LJ_DEBUG_VAR_DELIMETER_CHAR );
		if ( delimPtr != NULL )
		{
			count++;
			delimPtr++;
		}
	}
	return count;
}

LJ_int LJ_debugVarGetScope( const LJ_char* const name, const LJ_int depth, LJ_char* const scope, const LJ_int scopeMaxLength, const LJ_int fullScope )
{
    LJ_int scopeStart = 0;
    const LJ_char* scopeString = name + scopeStart;
    LJ_int delimCount = 0;
    LJ_int index = 0;

    if ( depth < 0 )
    {
        scope[0] = '\0';
        return 0;
    }
    // Try to find the n'th delimeter in this string
    while ( delimCount < depth )
    {
        const LJ_char* delimPtr = strchr( scopeString, LJ_DEBUG_VAR_DELIMETER_CHAR );
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
        const LJ_char inputChar = scopeString[index];
        // Stop if we find the end of the string
        if ( inputChar == '\0' )
        {
            break;
        }
        // Stop if find the n'th delimeter
        if ( inputChar == LJ_DEBUG_VAR_DELIMETER_CHAR )
        {
			if ( fullScope == 1 )
			{
				delimCount++;
				if ( delimCount > depth )
				{
					break;
				}
			}
			else
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
void LJ_debugVarSort( void )
{
    // Very basic bubble sort
	LJ_int i;
	LJ_int j;
    const LJ_int numVars = g_debugVarNum;
#if DEBUGVAR_SORT_DEBUG
	printf( "---------------------------------------\n" );
	printf( "debugVarSort %d\n", numVars );
    for ( i = 0; i < numVars; i++ )
    {
		LJ_debugVarDef* const debugVarI = &g_debugVarArray[i];
		printf( "Var[%d/%d] '%s'\n", i, numVars, debugVarI->name );
	}
	printf( "---------------------------------------\n" );
#endif // #if DEBUGVAR_SORT_DEBUG
    for ( i = 0; i < ( numVars - 1 ); i++ )
    {
        for ( j = ( i + 1 ); j < numVars; j++ )
        {
            LJ_debugVarDef* const debugVarI = &g_debugVarArray[i];
            LJ_debugVarDef* const debugVarJ = &g_debugVarArray[j];

			// Don't do ordinary alpha sort
			// Compare the scopes one at a time and sort by the scope alpha value
			// put items with no matching scope below items with a scope
			const LJ_int depthI = LJ_debugVarCountDepth( debugVarI->name );
			const LJ_int depthJ = LJ_debugVarCountDepth( debugVarJ->name );
			const LJ_int minDepth = ( depthI < depthJ ) ? depthI : depthJ;

			LJ_int swap = 0;
			LJ_int d;
#if DEBUGVAR_SORT_DEBUG
			printf( "---------------------------------------\n" );
			printf( "DepthI %d Var '%s'\n", depthI, debugVarI->name );
			printf( "DepthJ %d Var '%s'\n", depthJ, debugVarJ->name );
			printf( "minDepth %d\n", minDepth );
#endif // #if DEBUGVAR_SORT_DEBUG
			for ( d = 0; d <= minDepth; d++ )
			{
				LJ_int scopeMatch = 0;
				LJ_char scopeI[LJ_DEBUG_VAR_MAX_NAME_LENGTH];
				LJ_char scopeJ[LJ_DEBUG_VAR_MAX_NAME_LENGTH];
				LJ_debugVarGetScope( debugVarI->name, d, scopeI, LJ_DEBUG_VAR_MAX_NAME_LENGTH, 0 );
				LJ_debugVarGetScope( debugVarJ->name, d, scopeJ, LJ_DEBUG_VAR_MAX_NAME_LENGTH, 0 );
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
              	const LJ_debugVarDef temp = *debugVarI;
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
		LJ_debugVarDef* const debugVarI = &g_debugVarArray[i];
		printf( "Var[%d/%d] '%s'\n", i, numVars, debugVarI->name );
	}
	printf( "***************************************\n" );
#endif // #if DEBUGVAR_SORT_DEBUG
}

LJ_debugVarDef* LJ_debugVarFindVar( const LJ_debugVarDef* const currentVar, 
							  const LJ_char* const parentScope, const LJ_char* const currentScope, 
							  const LJ_int depth, const LJ_int direction )
{
	const LJ_int startIndex = currentVar - g_debugVarArray;
	const LJ_int numVars = g_debugVarNum;
	const LJ_int endIndex = ( direction > 0 ) ? numVars : -1; 
	LJ_debugVarDef* newScopeVar = NULL;
	LJ_char testScope[LJ_DEBUG_VAR_MAX_NAME_LENGTH];
	LJ_int i;
	LJ_debugVarDef* lastVar = NULL;

	strncpy( testScope, currentScope, LJ_DEBUG_VAR_MAX_NAME_LENGTH );

	for ( i = startIndex; i != endIndex; i+= direction )
	{
		LJ_char varParentScope[LJ_DEBUG_VAR_MAX_NAME_LENGTH];
		LJ_debugVarDef* const debugVar = &g_debugVarArray[i];
		// Get the parent scope of this variable
		LJ_debugVarGetScope( debugVar->name, (depth-1), varParentScope, LJ_DEBUG_VAR_MAX_NAME_LENGTH, 1 );
        // Only consider things with the same parent scope
        if ( strcmp( parentScope, varParentScope ) == 0 )
        {
            LJ_char varScope[LJ_DEBUG_VAR_MAX_NAME_LENGTH];
			// Get the current scope of this variable
            if ( LJ_debugVarGetScope( debugVar->name, depth, varScope, LJ_DEBUG_VAR_MAX_NAME_LENGTH, 0 ) == 1 )
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
						strncpy( testScope, varScope, LJ_DEBUG_VAR_MAX_NAME_LENGTH );
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

LJ_debugVarDef* LJ_debugVarFindVarAtScope( const LJ_debugVarDef* const currentVar, const LJ_int depth, const LJ_int direction )
{
    LJ_char parentScopeString[LJ_DEBUG_VAR_MAX_NAME_LENGTH];
    LJ_char currentScopeString[LJ_DEBUG_VAR_MAX_NAME_LENGTH];

    LJ_debugVarGetScope( currentVar->name, (depth-1), parentScopeString, LJ_DEBUG_VAR_MAX_NAME_LENGTH, 1 );
    //printf( "parentScopeString : %s\n", parentScopeString );
    LJ_debugVarGetScope( currentVar->name, depth, currentScopeString, LJ_DEBUG_VAR_MAX_NAME_LENGTH, 0 );
    //printf( "currentScopeString : %s\n", currentScopeString );
	return LJ_debugVarFindVar( currentVar, parentScopeString, currentScopeString, depth, direction );
}

// Public functions
void LJ_debugVarInit( const LJ_int maxNumVars )
{
	g_debugVarMenuScrollStart = NULL;
	g_debugVarArray = NULL;
	g_debugVarNum = 0;
	g_debugVarMaxNum = maxNumVars;

	if ( maxNumVars > 0 )
	{
		g_debugVarArray = (LJ_debugVarDef*)LJ_debugVarMemAlloc( sizeof( LJ_debugVarDef ) * maxNumVars );
		g_debugVarMenuScrollStart = (LJ_float*)LJ_debugVarMemAlloc( sizeof( LJ_float ) * LJ_DEBUG_VAR_MAX_DEPTH );
	}

	LJ_debugVarReset();
}

void LJ_debugVarReset( void )
{
	g_selectedDepth = 0;
	g_editingMode = 0;
	g_selectedItem = g_debugVarArray;
	if ( g_debugVarMenuScrollStart != NULL )
	{
		LJ_int d;
		for ( d = 0; d < LJ_DEBUG_VAR_MAX_DEPTH; d++ )
		{
			g_debugVarMenuScrollStart[d] = 0.0f;
		}
	}
	g_intVariableAlterAmount = 1;
	g_floatVariableAlterAmount = 0.1f;
	g_menuSelectedPlace = 0.0f;
}

void LJ_debugVarShutdown( void )
{
	LJ_debugVarReset();
	g_debugVarNum = 0;
	LJ_debugVarMemFree( g_debugVarArray );
	LJ_debugVarMemFree( g_debugVarMenuScrollStart );
}

LJ_int LJ_debugVarRegister( const LJ_char* const name, const LJ_int type, void* dataPtr, const LJ_int flags )
{
    LJ_debugVarDef* const debugVar = &g_debugVarArray[g_debugVarNum];
    debugVar->name = name;
    debugVar->dataPtr = dataPtr;
    debugVar->flags = type | flags;
    g_debugVarNum++;

    LJ_debugVarSort();
    return 1;
}

#define DEBUGVAR_RENDER_DEBUG 0

void LJ_debugVarRender( void )
{
    LJ_int depth = 0;
	LJ_char tempString[LJ_DEBUG_VAR_MAX_NAME_LENGTH];
	LJ_char currentScope[LJ_DEBUG_VAR_MAX_NAME_LENGTH];
    const LJ_float xOrigin = 0.4f;
	const LJ_float yOrigin = 0.2f;
	LJ_float x0 = xOrigin;
	LJ_float y0 = yOrigin;
	LJ_float y1 = y0;
	LJ_float yStart = y0;
	LJ_float maxX = 0.0f;
	const LJ_uint bgColour = 0x000000FF; 
	const LJ_uint infoTextColour = 0xFFFFFFFF; 
	const LJ_int maxDepth = LJ_debugVarCountDepth( g_selectedItem->name );
	LJ_float xLen = 0.0f;
#if DEBUGVAR_RENDER_DEBUG
    printf( "-------------------------------\n" );
    printf( "SelectedIndex: %d SelectedDepth %d/%d\n", ( g_selectedItem - g_debugVarArray ), g_selectedDepth, maxDepth );
#endif // #if DEBUGVAR_RENDER_DEBUG

	tempString[0] = '\0';
	// Nice BG box to put info text in
	y1 += 1.5f;
	if ( g_editingMode == 1 )
	{
		// Get the length
		if ( g_selectedItem->flags & LJ_DEBUG_VAR_BOOL )
		{
			sprintf( tempString, "Edit Mode: BOOL value" );
		}
		else if ( g_selectedItem->flags & LJ_DEBUG_VAR_INT )
		{
			sprintf( tempString, "Edit Mode: SELECT to accept, CANCEL to abort: Inc: %d", g_intVariableAlterAmount );
		}
		else if ( g_selectedItem->flags & LJ_DEBUG_VAR_FLOAT )
		{
			sprintf( tempString, "Edit Mode: SELECT to accept, CANCEL to abort: Inc: %f", g_floatVariableAlterAmount );
		}
		xLen = LJ_debugVarRenderText( 0, x0, y0, infoTextColour, tempString );
	}
	else
	{
		if ( g_selectedDepth == maxDepth )
		{
			if ( g_selectedItem->flags & LJ_DEBUG_VAR_BOOL )
			{
				sprintf( tempString, "Menu Mode : SELECT to toggle BOOL variable" );
			}
			else
			{
				sprintf( tempString, "Menu Mode : Press SELECT to enter edit mode" );
			}
			if ( g_selectedItem->flags & LJ_DEBUG_VAR_READ_ONLY )
			{
				sprintf( tempString, "Menu Mode : READ ONLY value     " );
			}
		}
		else
		{
			sprintf( tempString, "Menu Mode    " );
		}
		xLen = LJ_debugVarRenderText( 0, x0, y0, infoTextColour, tempString );
	}
	LJ_debugVarDrawBackground( x0, y0, x0+xLen, y1, bgColour );
	// Draw the text
	LJ_debugVarRenderText( 1, x0+0.9f, y0, infoTextColour, tempString );

	yStart = y1 + 0.2f;

    while ( depth <= g_selectedDepth )
    {
		const LJ_float yMenuStart = g_debugVarMenuScrollStart[depth];
		const LJ_int numVars = g_debugVarNum;
		LJ_int i;
		LJ_int p;
    	x0 += maxX;
		maxX = 0.0f;
		y0 = yStart;

		// Two passes 
		// 1st pass work out size and draw background 
		// 2nd pass - draw the text
		for ( p = 0; p < 2; p++ )
		{
			const LJ_float yThisMenuStart = yStart;
			LJ_float yCount = 0.0f;
			LJ_float y = yStart;
			LJ_char parentScope[LJ_DEBUG_VAR_MAX_NAME_LENGTH];
			LJ_char selectedScope[LJ_DEBUG_VAR_MAX_NAME_LENGTH];
			currentScope[0] = '\0';

			// Get the parent scope of the selected item
			LJ_debugVarGetScope( g_selectedItem->name, (depth-1), parentScope, LJ_DEBUG_VAR_MAX_NAME_LENGTH, 1 );
			// Get the current scope of the selected item
			LJ_debugVarGetScope( g_selectedItem->name, (depth), selectedScope, LJ_DEBUG_VAR_MAX_NAME_LENGTH, 0 );

#if DEBUGVAR_RENDER_DEBUG
			printf( "Parent Scope: %s\n", parentScope );
			printf( "Selected Scope: %s\n", selectedScope );
#endif // #if DEBUGVAR_RENDER_DEBUG
			for ( i = 0; i < numVars; i++ )
			{
				const LJ_debugVarDef* const debugVar = &g_debugVarArray[i];
				// Get the parent scope of this variable
				LJ_char varParentScope[LJ_DEBUG_VAR_MAX_NAME_LENGTH];
				LJ_debugVarGetScope( debugVar->name, (depth-1), varParentScope, LJ_DEBUG_VAR_MAX_NAME_LENGTH, 1 );

				if ( ( y - yThisMenuStart ) >= g_menuMaxY )
				{
					continue;
				}
				// Ignore things with the wrong parent scope
				if ( strcmp( parentScope, varParentScope ) == 0 )
				{
					LJ_char varScope[LJ_DEBUG_VAR_MAX_NAME_LENGTH];
					if ( LJ_debugVarGetScope( debugVar->name, depth, varScope, LJ_DEBUG_VAR_MAX_NAME_LENGTH, 0 ) == 1 )
					{
						if ( strcmp( currentScope, varScope ) != 0 )
						{
							LJ_uint colour = 0x909090FF;
							LJ_char outputString[256];
							LJ_char finalOutputString[256];
							LJ_int selected = 0;
							LJ_int selected_colour = 0xFFFF00FF;
							const LJ_int varMaxDepth = LJ_debugVarCountDepth( debugVar->name );
							LJ_float endX = 0.0f;

							strncpy( currentScope, varScope, LJ_DEBUG_VAR_MAX_NAME_LENGTH );
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
#if DEBUGVAR_RENDER_DEBUG
			printf( "Var Scope: %s\n", varScope);
			printf( "OutputString: %s\n", outputString );
#endif // #if DEBUGVAR_RENDER_DEBUG
							if ( depth == varMaxDepth )
							{
								LJ_char outputValue[LJ_DEBUG_VAR_MAX_NAME_LENGTH];
								LJ_debugVarValue tempValue;
								LJ_debugVarGetValue( debugVar, &tempValue );
								LJ_debugVarPrintValue( debugVar->flags, &tempValue, outputValue );
								sprintf( finalOutputString, "%s: %s", outputString, outputValue );
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
								if ( debugVar->flags & LJ_DEBUG_VAR_READ_ONLY )
								{
									colour = 0xFF0000FF;
								}
							}
							else
							{
								strcpy(finalOutputString,outputString);
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
#if DEBUGVAR_RENDER_DEBUG
			printf( "finalOutputString: %d %s\n", p, finalOutputString );
#endif // #if DEBUGVAR_RENDER_DEBUG
								endX = LJ_debugVarRenderText( p, x0+0.8f, y, colour, finalOutputString ) + 2.9f;
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
				const LJ_float x1 = x0 + maxX - 0.4f;
				const LJ_float y1 = y + 1;
				LJ_debugVarDrawBackground( x0, y0, x1, y1, bgColour );
			}
		}
#if DEBUGVAR_DEBUG_MENU_POS
		sprintf( tempString, "start %3.1f", g_debugVarMenuScrollStart[depth] );
		LJ_debugVarRenderText( 1, x0, y1+5.0f, 0xFFFFFFFF, tempString );
#endif // #if DEBUGVAR_DEBUG_MENU_POS
        depth++;
    };
#if DEBUGVAR_DEBUG_MENU_POS
	sprintf( tempString, "g_menuSelectedPlace %f", g_menuSelectedPlace );
	LJ_debugVarRenderText( 1, 0.0f, 12.0f, 0xFFFFFFFF, tempString );
#endif // #if DEBUGVAR_DEBUG_MENU_POS
}

// Valid inputs are:
// LJ_DEBUG_VAR_INPUT_UP
// LJ_DEBUG_VAR_INPUT_DOWN
// LJ_DEBUG_VAR_INPUT_LEFT
// LJ_DEBUG_VAR_INPUT_RIGHT
// LJ_DEBUG_VAR_INPUT_SELECT
// LJ_DEBUG_VAR_INPUT_CANCEL
void LJ_debugVarInput( const LJ_int debugInputKey )
{
	const LJ_int fastKey = debugInputKey & LJ_DEBUG_VAR_INPUT_FAST;
	const LJ_int debugKey = debugInputKey & 0xFFFF;
	LJ_int newVarDirection = 0;
	LJ_int maxDepth = 0;
    LJ_int selectedIndex = 0;
	const LJ_int incAmt = ( fastKey ) ? 10 : 1;
	const LJ_int varMaxDepth = LJ_debugVarCountDepth( g_selectedItem->name );
    // Need to make the inputs match the menu display!
    if ( debugKey == LJ_DEBUG_VAR_INPUT_UP )
    {
		if ( ( g_selectedDepth == varMaxDepth ) && ( g_editingMode == 1 ) )
		{
			// If in editing mode then increment the value
			LJ_debugVarAlterValue( +incAmt, g_selectedItem );
		}
		else
		{
			// Otherwise go up the menu
			newVarDirection = -1;
		}
    }
    if ( debugKey == LJ_DEBUG_VAR_INPUT_DOWN )
    {
		if ( ( g_selectedDepth == varMaxDepth ) && ( g_editingMode == 1 ) )
		{
			// If in editing mode then decrement the value
			LJ_debugVarAlterValue( -incAmt, g_selectedItem );
		}
		else
		{
			// Otherwise go down the menu
			newVarDirection = +1;
		}
	}
    if ( debugKey == LJ_DEBUG_VAR_INPUT_SELECT )
    {
		// If the currently selected item is a variable then SELECT enters/exits edit mode
		if ( g_selectedDepth == varMaxDepth )
		{
			if ( g_editingMode == 1 )
			{
				LJ_debugVarEndEditMode( 0 );
			}
			else if ( ( g_selectedItem->flags & LJ_DEBUG_VAR_READ_ONLY ) == 0 )
			{
				// Don't enter edit mode for bool vars (just toggle them)
				if ( g_selectedItem->flags & LJ_DEBUG_VAR_BOOL )
				{
					LJ_debugVarAlterValue( +1, g_selectedItem );
				}
				else
				{
					LJ_debugVarStartEditMode();
				}
			}
		}
		else
		{
        	g_selectedDepth++;
		}
    }
    if ( debugKey == LJ_DEBUG_VAR_INPUT_CANCEL )
    {
		// If in editing mode then CANCEL stops editing and puts back the old value
		if ( ( g_selectedDepth == varMaxDepth ) && ( g_editingMode == 1 ) )
		{
			LJ_debugVarEndEditMode( 1 );
		}
		else
		{
        	g_selectedDepth--;
		}
    }
    if ( debugKey == LJ_DEBUG_VAR_INPUT_RIGHT )
    {
		if ( ( g_selectedDepth == varMaxDepth ) && ( g_editingMode == 1 ) )
		{
			// If in editing mode then increase the increment value
			LJ_debugVarAlterIncValue( +incAmt );
		}
		else
		{
        	g_selectedDepth++;
		}
    }
    if ( debugKey == LJ_DEBUG_VAR_INPUT_LEFT )
    {
		if ( ( g_selectedDepth == varMaxDepth ) && ( g_editingMode == 1 ) )
		{
			LJ_debugVarAlterIncValue( -incAmt );
		}
		else
		{
        	g_selectedDepth--;
		}
	}
	if ( newVarDirection != 0 )
	{
		const LJ_debugVarDef* const currentVar = g_selectedItem;
		LJ_debugVarDef* const newVar = LJ_debugVarFindVarAtScope( currentVar, g_selectedDepth, newVarDirection );
		if ( newVar != NULL )
		{
        	g_selectedItem = newVar;
			if ( newVar != currentVar )
			{
				LJ_int depth;
				const LJ_float newSelectedPlace = g_menuSelectedPlace + newVarDirection;
				if ( newSelectedPlace < 0.0f )
				{
					g_debugVarMenuScrollStart[g_selectedDepth] -= 1.0f;
				}
				else if ( newSelectedPlace >= g_menuMaxY )
				{
					g_debugVarMenuScrollStart[g_selectedDepth] += 1.0f;
				}
				// Reset where the sub-menus start
				for ( depth = g_selectedDepth+1; depth < LJ_DEBUG_VAR_MAX_DEPTH; depth++ )
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
    if ( g_selectedDepth > LJ_DEBUG_VAR_MAX_DEPTH )
    {
        g_selectedDepth  = LJ_DEBUG_VAR_MAX_DEPTH;
    }
	maxDepth = LJ_debugVarCountDepth( g_selectedItem->name );
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

