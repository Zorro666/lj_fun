#include "LJ.h"
#include "LJ_unittest.h"
#include "LJ_LibCommon/LJ_LibCommon.h"

#if LJ_USE_UNITTEST

LJ_unittestTest s_unittestTestsHead;
LJ_unittestTest* s_unittestTestsTailPtr;

void LJ_unittestInit( void )
{
    LJ_unittestReset();
	LJ_unittestRegisterExternalTests();
}

void LJ_unittestReset( void )
{
    s_unittestTestsHead.nextPtr = LJ_NULL;
    s_unittestTestsHead.func = LJ_NULL;
    s_unittestTestsHead.groupName = LJ_NULL;
    s_unittestTestsHead.testName = LJ_NULL;

    s_unittestTestsTailPtr = &s_unittestTestsHead;
}

void LJ_unittestShutdown( void )
{
    LJ_unittestReset();
}

// Loop over the linked list of registered unit tests and run them
void LJ_unittestTick( void )
{
	LJ_uint numFailedTests = 0;
	LJ_uint numTests = 0;
	const LJ_unittestTest* prevTest = LJ_NULL;
	const LJ_unittestTest* test = s_unittestTestsHead.nextPtr;
	const LJ_char* groupName = "__SENTINEL__";
	LJ_uint numFailedTestsGroup = 0;
	LJ_uint numTestsGroup = 0;
	LJ_outputPrintGold( ( "\n" ) );
	LJ_outputPrintGold( ( "LJ_unittest: Starting\n" ) );
	while ( test != LJ_NULL )
	{
		if ( LJ_strIsSameIgnoreCase( groupName, test->groupName ) == LJ_FALSE )
		{
			// A new test group output info about the last test group
			if ( prevTest != LJ_NULL )
			{
				if ( numFailedTestsGroup != 0 )
				{
					LJ_outputPrintGold( ( "LJ_unittest: Finished group[%s] %d out of %d tests failed\n", 
									   	   test->groupName, numFailedTestsGroup, numTestsGroup ) );
				}
				else
				{
					LJ_outputPrintGold( ( "LJ_unittest: Finished group[%s] %d tests passed successfully\n", 
								   		   prevTest->groupName, numTestsGroup ) );
				}
			}
			numFailedTestsGroup = 0;
			numTestsGroup = 0;
			prevTest = test;
			groupName = prevTest->groupName;
			LJ_outputPrintGold( ( "\n" ) );
			LJ_outputPrintGold( ( "LJ_unittest: Starting group[%s]\n", test->groupName ) );
		}

		// Run this test
		numTests++;
		numTestsGroup++;
		if ( test->func() == LJ_FALSE )
		{
			numFailedTestsGroup++;
			numFailedTests++;
		}
		test = test->nextPtr;
	}
	// A new test group output info about the last test group
	if ( prevTest != LJ_NULL )
	{
		if ( numFailedTestsGroup != 0 )
		{
			LJ_outputPrintGold( ( "LJ_unittest: Finished group[%s] %d out of %d tests failed\n", 
							   	   prevTest->groupName, numFailedTestsGroup, numTestsGroup ) );
		}
		else
		{
			LJ_outputPrintGold( ( "LJ_unittest: Finished group[%s] %d tests passed successfully\n", 
								   prevTest->groupName, numTestsGroup ) );
		}
	}

	LJ_outputPrintGold( ( "\n" ) );
	if ( numFailedTests > 0 )
	{
		LJ_outputPrintGold( ( "LJ_unittest: Finished %d out of %d tests failed\n", numFailedTests, numTests ) );
	}
	else
	{
		LJ_outputPrintGold( ( "LJ_unittest: Finished All %d tests passed successfully\n", numTests ) );
	}
	LJ_outputPrintGold( ( "\n" ) );
}

LJ_int LJ_unittestRegister( LJ_unittestTest* const testData, 
							const LJ_char* const groupName, const LJ_char* const testName, 
							LJ_unittestFunc testFunc )
{
    // Add to linked list of unit tests to run
    // Want to make sure we loop through the tests in the order they are registered
    s_unittestTestsTailPtr->nextPtr = testData;
    testData->nextPtr = LJ_NULL;
    testData->groupName = groupName;
    testData->testName = testName;
    testData->func = testFunc;
    s_unittestTestsTailPtr = testData;
	
	LJ_outputPrintGold( ( "LJ_unittest: Registering[%s:%s] data:0x%X func:0x%X\n", 
						 groupName, testName, (LJ_uint)testData, (LJ_uint)testFunc ) );
	return 0;
}

void LJ_unittestFailure( const LJ_char* const groupName, const LJ_char* const testName, 
						 const LJ_char* const file, const LJ_uint line,
						 const LJ_char* const expr, ... )
{
	#define LJ_UNITTEST_MAX_BUFFER 1024
	LJ_char output[LJ_UNITTEST_MAX_BUFFER];
	LJ_valist args;
	LJ_VA_START( args, expr );
	LJ_strVSPrint( output, LJ_UNITTEST_MAX_BUFFER, expr, &args );
	LJ_outputPrintGold( ( "LJ_unittest: Error [%s:%s] %s File:%s:%d\n", groupName, testName, output, file, line ) );
}

#endif // #if LJ_USE_UNITTEST

