#include "LJ.h"
#include "LJ_LibCommon/LJ_LibCommon.h"

#include "RS_renderSort.h"
#include "LJ_LibDebug/LJ_LibDebug.h"

typedef struct BatchItem_s
{
	int nextBatchItemIndex;
	int instanceDataStart;
	int instanceDataEnd;
} BatchItem;

typedef struct InstanceData_s
{
	float x;
	float y;
	float z;
	float size;
} InstanceData;

#define MAX_NUM_INSTANCES (20000)
#define MAX_NUM_BATCHES (1000)
#define MAX_NUM_TYPES (8)

// The actual render instance data
InstanceData s_instances[MAX_NUM_INSTANCES];

// The data about each render batch e.g. start index, num instances (the batch type is already known)
BatchItem s_batchData[MAX_NUM_BATCHES];

// The linked list start of the render batch per type - so we can render from the start of list
BatchItem s_batchHeads[MAX_NUM_TYPES];

// The linked list end of the render batch per type - so we add to end of list all the time
BatchItem* s_batchTails[MAX_NUM_TYPES];

int s_numBatchItems = 0;
BatchItem* s_currentBatchItem = s_batchData;

int s_numInstances = 0;
InstanceData* s_currentInstanceData = s_instances;

int s_maxNumTiles = 0;
int s_maxNumTypesPerTile = 0;
int s_maxNumInstancesPerTypePerTile = 0;
float s_instanceXRange = 5.0f;
float s_instanceYRange = 5.0f;
float s_instanceZRange = 5.0f;

void addInstance( const InstanceData* const instanceData )
{
	s_currentInstanceData[0] = instanceData[0];
	//LJ_outputPrintDebug( ( "%d x %f y %f z %f size %f\n", s_numInstances, instanceData->x, instanceData->y, instanceData->z, instanceData->size ) );
	s_currentInstanceData++;
}

void addBatch( const int type, const int instStartIndex, const int numInstInBatch )
{
	BatchItem* const tail = s_batchTails[type];
	int tailEndInstance;

	// Ignore 0 length batches
	if ( numInstInBatch == 0 )
	{
		return;
	}

	// If starting on the head of the batch just reuse it
	if ( tail->nextBatchItemIndex == -2 )
	{
		tail->nextBatchItemIndex = -1;
		tail->instanceDataStart = instStartIndex;
		tail->instanceDataEnd = instStartIndex + numInstInBatch;
		//LJ_outputPrintDebug( ( "addBatch type[%d] start %d num %d current %d started from head\n", type, instStartIndex, numInstInBatch, s_numBatchItems ) );
		return;
	}

	// If the tail of the batch ends where this batch starts then just add this onto it
	tailEndInstance = tail->instanceDataEnd;
	if ( tailEndInstance == instStartIndex )
	{
		tail->instanceDataEnd += numInstInBatch;
		//LJ_outputPrintDebug( ( "addBatch type[%d] start %d num %d current %d made tail longer\n", type, instStartIndex, numInstInBatch, s_numBatchItems ) );
		return;
	}

	// Need a new batch entry
	s_currentBatchItem->nextBatchItemIndex = -1;
	s_currentBatchItem->instanceDataStart = instStartIndex;
	s_currentBatchItem->instanceDataEnd = instStartIndex + numInstInBatch;

	// Update the last tail to point to this batch
	tail->nextBatchItemIndex = s_numBatchItems;

	// We are now the new tail
	s_batchTails[type] = s_currentBatchItem;
	
	//LJ_outputPrintDebug( ( "addBatch type[%d] start %d num %d current %d\n", type, instStartIndex, numInstInBatch, s_numBatchItems ) );
	// One more batch in the list
	s_currentBatchItem++;
	s_numBatchItems++;
}

int createInstances( void )
{
	int tile;
	int numTiles;
	int totalNumInstances = 0;
	const float xRange = s_instanceXRange;
	const float yRange = s_instanceYRange;
	const float zRange = s_instanceZRange;

	// Do a random number of tiles
	numTiles = LJ_mathGetRandMax( s_maxNumTiles );
	LJ_outputPrintDebug( ( "numTiles = %d\n", numTiles ) );
	for ( tile = 0; tile < numTiles; tile++ )
	{
		// per tile random number of types
		int type; 
		int numTypes; 
		LJ_mathSeedRand( tile * 764 + 43243 );
		numTypes = LJ_mathGetRandMax( s_maxNumTypesPerTile );
		LJ_outputPrintDebug( ( "tile[%d] numTypes = %d\n", tile, numTypes ) );
		for ( type = 0; type < numTypes; type++ )
		{
			// Then per type random number of instances
			int inst;
			int numInstInBatch = 0;
			int instStartIndex = totalNumInstances;
			const int numInstances = LJ_mathGetRandMax( s_maxNumInstancesPerTypePerTile );
			LJ_mathSeedRand( tile * 373 + type * 235 + 8673 );
			for ( inst = 0; inst < numInstances; inst++ )
			{
				InstanceData instanceData;
				// A new instance
				float x = LJ_mathGetRandRangeFloat( -xRange, +xRange );
				float y = LJ_mathGetRandRangeFloat( -yRange, +yRange );
				float z = LJ_mathGetRandRangeFloat( -zRange, +zRange );
				float size = LJ_mathGetRandRangeFloat( 0.2f, 2.0f );

				instanceData.x = x;
				instanceData.y = y;
				instanceData.z = z;
				instanceData.size = size;
				// Add the instance
				addInstance( &instanceData );

				totalNumInstances++;
				numInstInBatch++;
			}
			addBatch( type, instStartIndex, numInstInBatch );
		}
	}

	return totalNumInstances;
}

extern void debugDrawSphere( float x, float y, float z, float size, int colour );

void renderInstances( void )
{
	const int numInstances = s_numInstances;
	const InstanceData* instanceDataPtr = s_instances;
	int inst;
	LJ_mathSeedRand( 23423 );
	for ( inst = 0; inst < numInstances; inst++ )
	{
		const float x = instanceDataPtr->x;
		const float y = instanceDataPtr->y;
		const float z = instanceDataPtr->z;
		const float size = instanceDataPtr->size;

		const unsigned int colour = LJ_mathGetRand32();
		debugDrawSphere( x, y, z, size, colour );
		instanceDataPtr++;
	}
}

void renderBatches( void )
{
	int numBatches = 0;
	int numTotalInstances = 0;
	int type;

	for ( type = 0; type < MAX_NUM_TYPES; type++ )
	{
		const BatchItem* batchData = &s_batchHeads[type];
		// Render all in this type
		int numBatchesInType = 0;
		int numInstancesInType = 0;
		LJ_mathSeedRand( type * 3543 + 2573 );
		do
		{
			// So have a batch to render
			const int instanceDataStart = batchData->instanceDataStart;
			const int instanceDataEnd = batchData->instanceDataEnd;
			const int numInstancesInBatch = instanceDataEnd - instanceDataStart;
			const int nextBatchItemIndex = batchData->nextBatchItemIndex;
			// From instance index start -> start + num
			int inst;
			const InstanceData* instanceDataPtr = s_instances + instanceDataStart;
			for ( inst = 0; inst < numInstancesInBatch; inst++ )
			{
				const float x = instanceDataPtr->x;
				const float y = instanceDataPtr->y;
				const float z = instanceDataPtr->z;
				const float size = instanceDataPtr->size;

				const unsigned int colour = LJ_mathGetRand32();
				debugDrawSphere( x, y, z, size, colour );
				instanceDataPtr++;
			}
			if ( numInstancesInBatch > 0 )
			{
				numBatchesInType++;
			}
			numInstancesInType += numInstancesInBatch;

			// if we are at the end of the list end this batch list
			if ( nextBatchItemIndex < 0 )
			{
				break;
			}
			batchData = &s_batchData[nextBatchItemIndex];
		}
		while ( 1 );
		numBatches += numBatchesInType;
		if ( numBatchesInType > 0 )
		{
			s_numBatchItems++;
		}
		numTotalInstances += numInstancesInType;
		//LJ_outputPrintGold( ( "Type[%d] numBatches[%d] numInstances[%d]\n", type, numBatchesInType, numInstancesInType ) );
	}
	//LJ_outputPrintGold( ( "%d vs %d\n", numBatches, s_numBatchItems ) );
}

void renderSortInit( void )
{
	s_maxNumTiles = 8;
	s_maxNumTypesPerTile = 2;
	s_maxNumInstancesPerTypePerTile = 8;
    LJ_debugVarRegister( "RenderSort:maxNumTiles", LJ_DEBUG_VAR_INT, &s_maxNumTiles, 0 );
    LJ_debugVarRegister( "RenderSort:maxNumTypesPerTile", LJ_DEBUG_VAR_INT, &s_maxNumTypesPerTile, 0 );
    LJ_debugVarRegister( "RenderSort:maxNumInstancesPerTypePerTile", LJ_DEBUG_VAR_INT, &s_maxNumInstancesPerTypePerTile, 0 );
    LJ_debugVarRegister( "RenderSort:numInstances", LJ_DEBUG_VAR_INT | LJ_DEBUG_VAR_READ_ONLY , &s_numInstances, 0 );
    LJ_debugVarRegister( "RenderSort:numBatchItems", LJ_DEBUG_VAR_INT | LJ_DEBUG_VAR_READ_ONLY , &s_numBatchItems, 0 );
    LJ_debugVarRegister( "RenderSort:XRange", LJ_DEBUG_VAR_FLOAT, &s_instanceXRange, 0 );
    LJ_debugVarRegister( "RenderSort:YRange", LJ_DEBUG_VAR_FLOAT, &s_instanceYRange, 0 );
    LJ_debugVarRegister( "RenderSort:ZRange", LJ_DEBUG_VAR_FLOAT, &s_instanceZRange, 0 );
}

void renderSortTest( void )
{
	int numInstances = 0;
	int type;

	s_numInstances = 0;
	s_currentInstanceData = s_instances;
	s_numBatchItems = 0;
	s_currentBatchItem = s_batchData;

	// Set the batch tails to point to the batch heads
	for ( type = 0; type < MAX_NUM_TYPES; type++ )
	{
		s_batchTails[type] = &s_batchHeads[type];
		// The head is empty
		s_batchHeads[type].nextBatchItemIndex = -2;
		s_batchHeads[type].instanceDataStart = 0;
		s_batchHeads[type].instanceDataEnd = 0;
	}

	LJ_mathSeedRand( 763 );
	numInstances = createInstances();

	s_numInstances = numInstances;
}

void renderSortDraw( void )
{
	//renderInstances();
	renderBatches();
}

