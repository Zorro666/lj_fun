#include "LJ.h"
#include "LJ_unittest.h"
#include "LJ_LibCommon/LJ_LibCommon.h"

#if LJ_USE_UNITTEST

LJ_UNITTEST_FUNCTION_START( str, basic )
{
	LJ_UNITTEST_TRUE( ( LJ_strCompare( "abc", "abc") == 0 ) );
	LJ_UNITTEST_EQUALS( LJ_strGetLength( "abc" ) , 3 );
}
LJ_UNITTEST_FUNCTION_END( str, basic )

#endif // #if LJ_USE_UNITTEST

