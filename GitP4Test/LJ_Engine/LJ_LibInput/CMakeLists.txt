# add definitions, compiler switches, etc.
ADD_DEFINITIONS( ${GLOBAL_DEFINITIONS} )
INCLUDE_DIRECTORIES( ${GLOBAL_INCLUDE_DIRECTORIES} ${LJ_ENGINE_DIR}/LJ_LibInput )

SET( CMAKE_VERBOSE_MAKEFILE OFF )

# list all source files here
ADD_LIBRARY( LJ_LibInput STATIC LJ_input.c LJ_key.c LJ_mouse.c )

