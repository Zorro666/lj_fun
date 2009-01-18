PROJECT( ${LJ_PROJECT} C )

SET( LJ_ROOT_DIR ${${LJ_PROJECT}_SOURCE_DIR} )
SET( LJ_ENGINE_DIR ${LJ_ROOT_DIR}/LJ_Engine )

SET( GLUT_ROOT_PATH $ENV{GLUT_ROOT_PATH} )
SET( SDL_BUILDING_LIBRARY TRUE)

Find_Package( OpenGL REQUIRED )
Find_Package( SDL REQUIRED )
Find_Package( GLUT REQUIRED )

IF ( NOT SDL_FOUND )
    MESSAGE( FATAL_ERROR "SDL not found" )
ENDIF ( NOT SDL_FOUND )

IF ( NOT GLUT_FOUND )
    MESSAGE( FATAL_ERROR "GLUT not found" )
ENDIF ( NOT GLUT_FOUND )

IF ( NOT OPENGL_FOUND )
    MESSAGE( FATAL_ERROR "OPENGL not found" )
ENDIF ( NOT OPENGL_FOUND )

MESSAGE( "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
MESSAGE( "SDL_LIBRARY = ${SDL_LIBRARY}" )
MESSAGE( "GLUT_LIBRARY = ${GLUT_LIBRARY}" )
MESSAGE( "OPENGL_LIBRARY = ${OPENGL_LIBRARY}" )
MESSAGE( "GL_LIBRARY = ${GL_LIBRARY}" )

SET( GLOBAL_DEFINITIONS "" )
SET( GLOBAL_INCLUDE_DIRECTORIES "" )
SET( GLOBAL_LINK_DIRECTORIES "" )

# Global definitions
IF ( MSVC )

    SET( GLOBAL_CFLAGS -W4 -WX -DPLATFORM_PC )

ELSE ( MSVC )

    SET( GLOBAL_CFLAGS -Wextra -Wall -Werror -Wdeclaration-after-statement -fstrict-aliasing -DPLATFORM_LINUX )

ENDIF ( MSVC )

IF ( CMAKE_BUILD_TYPE STREQUAL "DEBUG" )

    SET( CONFIG_CFLAGS -DLJ_DEBUG )

    IF ( MSVC )
	SET( CONFIG_CFLAGS ${CONFIG_CFLAGS} -Od -RTCsu -Zi )
    ELSE ( MSVC )
	SET( CONFIG_CFLAGS ${CONFIG_CFLAGS} -g )
    ENDIF ( MSVC )

ENDIF ( CMAKE_BUILD_TYPE STREQUAL "DEBUG" )

IF ( CMAKE_BUILD_TYPE STREQUAL "RELEASE" )

    SET( CONFIG_CFLAGS -DLJ_GOLD -DLJ_NDEBUG )

    IF ( MSVC )
	SET( CONFIG_CFLAGS ${CONFIG_CFLAGS} -O2 )
    ELSE ( MSVC )
	SET( CONFIG_CFLAGS ${CONFIG_CFLAGS} -O4 -ffast-math )
    ENDIF ( MSVC )

ENDIF ( CMAKE_BUILD_TYPE STREQUAL "RELEASE" )

IF ( CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO" )

    SET( CONFIG_CFLAGS -DLJ_NDEBUG )

    IF ( MSVC )
	SET( CONFIG_CFLAGS ${CONFIG_CFLAGS} -O2 -Zi )
    ELSE ( MSVC )
	SET( CONFIG_CFLAGS ${CONFIG_CFLAGS} -g -O2 )
    ENDIF ( MSVC )

ENDIF ( CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO" )

SET( GLOBAL_INCLUDE_DIRECTORIES ${GLOBAL_INCLUDE_DIRECTORIES} ${LJ_ENGINE_DIR} )
SET( GLOBAL_INCLUDE_DIRECTORIES ${GLOBAL_INCLUDE_DIRECTORIES} ${SDL_INCLUDE_DIR} )
SET( GLOBAL_INCLUDE_DIRECTORIES ${GLOBAL_INCLUDE_DIRECTORIES} ${GLUT_INCLUDE_DIR} )

SET( GLOBAL_DEFINITIONS ${GLOBAL_CFLAGS} ${CONFIG_CFLAGS} )

MESSAGE( "LJ_PROJECT = ${LJ_PROJECT}" )
MESSAGE( "LJ_ROOT_DIR = ${LJ_ROOT_DIR}" )
MESSAGE( "LJ_ENGINE_DIR = ${LJ_ENGINE_DIR}" )

MESSAGE( "GLOBAL_DEFINITIONS = ${GLOBAL_DEFINITIONS}" )
MESSAGE( "CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}" )
MESSAGE( "CMAKE_C_FLAGS_DEBUG = ${CMAKE_C_FLAGS_DEBUG}" )

MESSAGE( "GLOBAL_INCLUDE_DIRECTORIES = ${GLOBAL_INCLUDE_DIRECTORIES}" )
MESSAGE( "GLOBAL_LINK_DIRECTORIES = ${GLOBAL_LINK_DIRECTORIES}" )


