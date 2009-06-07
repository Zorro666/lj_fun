#ifndef LJ_HH
#define LJ_HH

////////////////////////////////////////////////////////////////
//
// The root base include file for LJ files and projects using the LJ system
// All source files must include this file first (before any other include files)
// This file sets config vars for the LJ system and 
// #includes the project header (LJ_project.h) so projects can control the configuration on a per project basis
//
////////////////////////////////////////////////////////////////

#include "LJ_project.h"
//
// Default memory tracking to be on in all builds except gold
#ifndef LJ_USE_MEM_TRACKING

#if !LJ_GOLD
#define LJ_USE_MEM_TRACKING			1
#endif // #if !LJ_GOLD

#endif // #ifndef LJ_USE_MEM_TRACKING

#endif // #ifndef LJ_HH
