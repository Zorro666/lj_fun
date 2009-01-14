#ifndef LJ_PROJECT_HH
#define LJ_PROJECT_HH

////////////////////////////////////////////////////////////////
//
// The root base include file for the UNITTEST project
// This file must only have settings to control the different config options
// e.g. the #define's to control conditional compilation of the LJ system
//
////////////////////////////////////////////////////////////////

#define LJ_USE_ASSERT 				1
#define LJ_USE_UNITTEST 			1

// To make the unittest failure define be an error/crash for this project
#define LJ_UNITTEST_FAILED()		{ *((LJ_int*)LJ_NULL) = 0; }

#endif // #ifndef LJ_PROJECT_HH
