# add definitions, compiler switches, etc.
ADD_DEFINITIONS( ${GLOBAL_DEFINITIONS} )
INCLUDE_DIRECTORIES( ${GLOBAL_INCLUDE_DIRECTORIES} )

SET( CMAKE_VERBOSE_MAKEFILE ON )

# list all source files here
ADD_LIBRARY( LJ_LibCommon LJ_Base/LJ_types.c LJ_Base/LJ_str.c )

