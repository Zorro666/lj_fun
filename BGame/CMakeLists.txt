# if you don't want the full compiler output, remove the following line
SET( CMAKE_VERBOSE_MAKEFILE OFF )

# add definitions, compiler switches, etc.
ADD_DEFINITIONS( ${GLOBAL_DEFINITIONS} )

INCLUDE_DIRECTORIES( ${GLOBAL_INCLUDE_DIRECTORIES} )
LINK_DIRECTORIES( ${GLOBAL_LINK_DIRECTORIES} )

# list all source files here
ADD_EXECUTABLE( a.out BG_main.c )

# need to link to some other libraries ? just add them here
#SET( ENGINE_LIBS LJ_LibCommon LJ_LibInput LJ_LibDebug LJ_LibEngine LJ_LibUnitTest )
SET( ENGINE_LIBS LJ_LibEngine LJ_LibUnitTest LJ_LibDebug LJ_LibInput LJ_LibCommon )
#TARGET_LINK_LIBRARIES( a.out ${ENGINE_LIBS} GL glut ${SDL_LIBRARY} )
TARGET_LINK_LIBRARIES( a.out ${ENGINE_LIBS} ${OPENGL_LIBRARY} ${GLUT_glut_LIBRARY} ${SDL_LIBRARY} )

