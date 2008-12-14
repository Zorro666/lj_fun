#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(${GLOBAL_DEFINITIONS})

INCLUDE_DIRECTORIES(${GLOBAL_INCLUDE_DIRECTORIES})
LINK_DIRECTORIES(${GLOBAL_LINK_DIRECTORIES})

#list all source files here
ADD_EXECUTABLE(a.out main.c renderSort.c)

#need to link to some other libraries ? just add them here
TARGET_LINK_LIBRARIES (a.out libdebug GL glut)

