#add definitions, compiler switches, etc.
ADD_DEFINITIONS( ${GLOBAL_DEFINITIONS} )
INCLUDE_DIRECTORIES( ${GLOBAL_INCLUDE_DIRECTORIES} ${LJ_ENGINE_DIR}/LJ_LibEngine )

SET( CMAKE_VERBOSE_MAKEFILE OFF )

#list all source files here
ADD_LIBRARY( LJ_LibEngine STATIC LJ_engine.c )

