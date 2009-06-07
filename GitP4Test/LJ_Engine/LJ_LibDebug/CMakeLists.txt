# add definitions, compiler switches, etc.
ADD_DEFINITIONS( ${GLOBAL_DEFINITIONS} )
INCLUDE_DIRECTORIES( ${GLOBAL_INCLUDE_DIRECTORIES} ${LJ_ENGINE_DIR}/LJ_LibDebug )

SET( CMAKE_VERBOSE_MAKEFILE OFF )

# list all source files here
ADD_LIBRARY( LJ_LibDebug STATIC LJ_DebugVar/LJ_debugVar.c )

