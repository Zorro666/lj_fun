# if you don't want the full compiler output, remove the following line
SET( CMAKE_VERBOSE_MAKEFILE OFF )

# add definitions, compiler switches, etc.
ADD_DEFINITIONS( ${GLOBAL_DEFINITIONS} )

INCLUDE_DIRECTORIES( ${GLOBAL_INCLUDE_DIRECTORIES} )

# list all source files here
ADD_EXECUTABLE( a.out BG_main.c )

# need to link to some other libraries ? just add them here
SET( ENGINE_LIBS LJ_LibCommon LJ_LibDebug LJ_LibInput LJ_LibEngine LJ_LibUnitTest )
TARGET_LINK_LIBRARIES( a.out ${ENGINE_LIBS} GL glut ${SDL_LIBRARY} )

