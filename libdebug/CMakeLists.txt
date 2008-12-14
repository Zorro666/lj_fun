#add definitions, compiler switches, etc.
ADD_DEFINITIONS(${GLOBAL_DEFINITIONS})

SET(CMAKE_VERBOSE_MAKEFILE OFF)

#list all source files here
ADD_LIBRARY(libdebug debugVar/debugVar.c)

