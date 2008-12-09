
PROJECT(renderSort)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -g -Werror -Wdeclaration-after-statement)

#list all source files here
ADD_EXECUTABLE(renderSort main.c ../libdebug/debugVar/debugVar.c renderSort.c)

#need to link to some other libraries ? just add them here
TARGET_LINK_LIBRARIES (renderSort GL glut)
