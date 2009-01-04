CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
PROJECT( BGAME )

Find_Package ( SDL REQUIRED )
Find_Package ( GLUT REQUIRED )
Find_Package ( SDL_image REQUIRED ) # if using SDL_image

# Workaround for the non-working REQUIRED flag
if ( NOT SDL_FOUND )
   message ( FATAL_ERROR "SDL not found!" )
endif ( NOT SDL_FOUND )

# Global definitions
SET( GLOBAL_CFLAGS -Wextra -Wall -Werror -Wdeclaration-after-statement -fstrict-aliasing )
SET( GLOBAL_DEFINITIONS ${GLOBAL_CFLAGS} ${CONFIG_CFLAGS} )

SET( GLOBAL_INCLUDE_DIRECTORIES )
SET( GLOBAL_INCLUDE_DIRECTORIES ${GLOBAL_INCLUDE_DIRECTORIES} ${BGAME_SOURCE_DIR}/LJ_Engine )
 
# if you don't want the full compiler output, remove the following line
SET( CMAKE_VERBOSE_MAKEFILE OFF )

ADD_SUBDIRECTORY( LJ_Engine/LJ_LibCommon )
ADD_SUBDIRECTORY( LJ_Engine/LJ_LibDebug )
ADD_SUBDIRECTORY( LJ_Engine/LJ_LibInput )
ADD_SUBDIRECTORY( LJ_Engine/LJ_LibEngine )
ADD_SUBDIRECTORY( BGame )
