CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
SET( LJ_PROJECT UNITTEST )

INCLUDE ( LJ_Engine/LJ_ROOT.cmake )

SET( GLOBAL_INCLUDE_DIRECTORIES ${GLOBAL_INCLUDE_DIRECTORIES} ${UNITTEST_SOURCE_DIR}/Unittest )
 
# if you don't want the full compiler output, remove the following line
SET( CMAKE_VERBOSE_MAKEFILE OFF )

ADD_SUBDIRECTORY( LJ_Engine/LJ_LibCommon )
ADD_SUBDIRECTORY( LJ_Engine/LJ_LibDebug )
ADD_SUBDIRECTORY( LJ_Engine/LJ_LibInput )
ADD_SUBDIRECTORY( LJ_Engine/LJ_LibEngine )
ADD_SUBDIRECTORY( LJ_Engine/LJ_LibUnitTest )
ADD_SUBDIRECTORY( LJ_Engine/LJ_LibGraphics )
ADD_SUBDIRECTORY( Unittest )
