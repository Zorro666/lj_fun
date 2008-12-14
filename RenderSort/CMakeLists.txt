#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE OFF)

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(${GLOBAL_DEFINITIONS})

INCLUDE_DIRECTORIES(${TEST_SOURCE_DIR}/libdebug)
LINK_DIRECTORIES(${TEST_BINARY_DIR}/libdebug)

#list all source files here
ADD_EXECUTABLE(renderSort main.c renderSort.c)

#need to link to some other libraries ? just add them here
TARGET_LINK_LIBRARIES (renderSort libdebug GL glut)

