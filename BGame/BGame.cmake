CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(BGAME)

Find_Package ( SDL REQUIRED )
Find_Package ( GLUT REQUIRED )
Find_Package ( SDL_image REQUIRED ) # if using SDL_image

# Workaround for the non-working REQUIRED flag
if ( NOT SDL_FOUND )
   message ( FATAL_ERROR "SDL not found!" )
endif ( NOT SDL_FOUND )

#Global definitions
SET(GLOBAL_CFLAGS -Wall -Werror -Wdeclaration-after-statement)
SET(GLOBAL_DEFINITIONS ${GLOBAL_CFLAGS} ${CONFIG_CFLAGS})
SET(GLOBAL_INCLUDE_DIRECTORIES ${BGAME_SOURCE_DIR}/LJ_Engine/LJ_LibDebug)
SET(GLOBAL_LINK_DIRECTORIES ${BGAME_BINARY_DIR}/LJ_Engine/LJ_LibDebug)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE OFF)

ADD_SUBDIRECTORY(LJ_Engine/LJ_LibDebug)
ADD_SUBDIRECTORY(BGame)
