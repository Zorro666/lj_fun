#ifndef LJ_UNITTEST_HH
#define LJ_UNITTEST_HH

#if LJ_USE_UNITTEST

#include "LJ_LibCommon/LJ_LibCommon.h"

typedef LJ_bool(LJ_unittestFunc)( void );

typedef struct LJ_unittestTest
{
    struct LJ_unittestTest* nextPtr;
    LJ_unittestFunc* func;
	const LJ_char* groupName;
	const LJ_char* testName;
} LJ_unittestTest;

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Standard module functions (the tick function runs the unit tests registered with the system)
//
////////////////////////////////////////////////////////////////////////////////////////////

void LJ_unittestInit( void );
void LJ_unittestReset( void );
void LJ_unittestTick( void );
void LJ_unittestShutdown( void );

LJ_int LJ_unittestRegister( LJ_unittestTest* const testData, 
							const LJ_char* const groupName, const LJ_char* const testName, 
							LJ_unittestFunc testFunc );

void LJ_unittestFailure( const LJ_char* const groupName, const LJ_char* const testName, 
						 const LJ_char* const file, const LJ_uint line,
						 const LJ_char* const expr, ... );

// In LJ_unittest_tests.c
void LJ_unittestRegisterExternalTests( void );

/////////////////////////////////////////////////////////////////////////////////////////////
//
// Macros used to create unit tests and to be used within unit test functions
//
/////////////////////////////////////////////////////////////////////////////////////////////

#define LJ_UNITTEST_FUNCTION_START( group, test ) \
	LJ_unittestTest s_LJ_unittestTests_##group##test; \
	LJ_bool LJ_unittestTestsFunc_##group##test( void ) \
	{ \
		LJ_bool __LJ_unittest_result__ = LJ_TRUE; \
		const LJ_char* const __LJ_unittest_groupName__ = #group; \
		const LJ_char* const __LJ_unittest_testName__ = #test; \
		LJ_outputPrintGold( ( "LJ_unittest: Running[%s:%s]\n", __LJ_unittest_groupName__, __LJ_unittest_testName__ ) );

#define LJ_UNITTEST_FUNCTION_END( group, test ) \
		return __LJ_unittest_result__; \
	}

#define LJ_UNITTEST_REGISTER( jake, test ) \
	do \
	{ \
		extern LJ_unittestTest s_LJ_unittestTests_##jake##test ; \
		extern LJ_unittestFunc LJ_unittestTestsFunc_##jake##test ; \
		LJ_unittestRegister( &s_LJ_unittestTests_##jake##test, #jake, #test, &LJ_unittestTestsFunc_##jake##test ); \
	} while ( 0 );

#ifndef LJ_UNITTEST_FAILED
#define LJ_UNITTEST_FAILED() (void)(0)
#endif // #ifndef LJ_UNITTEST_FAILED

#define LJ_UNITTEST_TRUE( expr ) \
	if ( !( expr ) ) \
	{ \
		LJ_unittestFailure( __LJ_unittest_groupName__, __LJ_unittest_testName__, LJ_SOURCE_FILE, LJ_SOURCE_LINE, \
							#expr " failed " ); \
		__LJ_unittest_result__ = LJ_FALSE; \
		LJ_UNITTEST_FAILED(); \
	} 

#define LJ_UNITTEST_EQUALS( lhs, rhs ) \
	if ( ( lhs ) != ( rhs ) ) \
	{ \
		LJ_unittestFailure( __LJ_unittest_groupName__, __LJ_unittest_testName__, LJ_SOURCE_FILE, LJ_SOURCE_LINE, \
							"'" #lhs " == " #rhs "' failed '%d != %d'", lhs, rhs ); \
		__LJ_unittest_result__ = LJ_FALSE; \
		LJ_UNITTEST_FAILED(); \
	} 

#define LJ_UNITTEST_FLOAT_EQUALS( lhs, rhs, tolerance ) \
	if ( LJ_mathFabsf( ( lhs ) - ( rhs ) ) > tolerance ) \
	{ \
		LJ_unittestFailure( __LJ_unittest_groupName__, __LJ_unittest_testName__, LJ_SOURCE_FILE, LJ_SOURCE_LINE, \
							"'" #lhs " == " #rhs "' failed '%f != %f'", lhs, rhs ); \
		__LJ_unittest_result__ = LJ_FALSE; \
		LJ_UNITTEST_FAILED(); \
	} 

#else // #if LJ_USE_UNITTEST

#define LJ_unittestInit() (void)(0)
#define LJ_unittestReset() (void)(0)
#define LJ_unittestTick() (void)(0)
#define LJ_unittestShutdown() (void)(0)

#endif // #if LJ_USE_UNITTEST

#endif // #ifndef LJ_UNITTEST_HH

