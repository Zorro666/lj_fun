cmake_minimum_required(VERSION 2.6)

PROJECT(TEST)

#add definitions, compiler switches, etc.
SET(GLOBAL_DEFINITIONS -Wall -g -Werror -Wdeclaration-after-statement)

SET(CMAKE_VERBOSE_MAKEFILE OFF)

ADD_SUBDIRECTORY(libdebug)
ADD_SUBDIRECTORY(RenderSort)
