#include "LJ.h"
#include "LJ_unittest.h"
#include "LJ_LibCommon/LJ_LibCommon.h"

#if LJ_USE_UNITTEST

#define LJ_FILE_TEST_SIZE		(256)
#define	LJ_FILE_TEST_STRING		"I am a walrus"
#define LJ_FILE_TEST_FILENAME	"../test/unittest.test"

LJ_UNITTEST_FUNCTION_START( filesystem, basic )
{
	LJ_fileHandle fh = LJ_FILE_HANDLE_INVALID;
	LJ_char writeData[LJ_FILE_TEST_SIZE];
	LJ_char readData[LJ_FILE_TEST_SIZE];
	LJ_bool retVal;
	LJ_int size = -1;

	LJ_memZero( writeData, LJ_FILE_TEST_SIZE );
	LJ_memZero( readData, LJ_FILE_TEST_SIZE );

	// Write to a file 
	fh = LJ_filesystemOpen( LJ_FILE_TEST_FILENAME, LJ_FILE_MODE_WRITE );
	LJ_UNITTEST_TRUE( ( fh != LJ_FILE_HANDLE_INVALID ) );

	LJ_strCopy( writeData, LJ_FILE_TEST_STRING );
	retVal = LJ_filesystemWrite( fh, writeData, LJ_FILE_TEST_SIZE );
	LJ_UNITTEST_TRUE( retVal );

	retVal = LJ_filesystemClose( fh );
	LJ_UNITTEST_TRUE( retVal );

	// Try to open it
	fh = LJ_filesystemOpen( "../test/unittest.test", LJ_FILE_MODE_READ );
	LJ_UNITTEST_TRUE( ( fh != LJ_FILE_HANDLE_INVALID ) );

	// Then try to read from it
	retVal = LJ_filesystemRead( fh, readData, LJ_FILE_TEST_SIZE );
	LJ_UNITTEST_TRUE( retVal );

	LJ_UNITTEST_TRUE( LJ_strIsSame( writeData, readData ) );
	
	retVal = LJ_filesystemClose( fh );
	LJ_UNITTEST_TRUE( retVal );

	// Need to test the get file size function
	size = LJ_filesystemGetSize( LJ_FILE_TEST_FILENAME );
	LJ_UNITTEST_EQUALS( size, LJ_FILE_TEST_SIZE );
}
LJ_UNITTEST_FUNCTION_END( filesystem, basic )

#endif // #if LJ_USE_UNITTEST

