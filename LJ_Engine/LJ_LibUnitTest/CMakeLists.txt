# add definitions, compiler switches, etc.
ADD_DEFINITIONS( ${GLOBAL_DEFINITIONS} )
INCLUDE_DIRECTORIES( ${GLOBAL_INCLUDE_DIRECTORIES} )

SET( CMAKE_VERBOSE_MAKEFILE OFF )

# list all source files here
SET( EXTERNAL_TESTS LJ_unittest_str.c LJ_unittest_mem.c )
ADD_LIBRARY( LJ_LibUnitTest STATIC LJ_unittest.c LJ_unittest_tests.c ${EXTERNAL_TESTS} )

