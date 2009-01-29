# add definitions, compiler switches, etc.
ADD_DEFINITIONS( ${GLOBAL_DEFINITIONS} )
INCLUDE_DIRECTORIES( ${GLOBAL_INCLUDE_DIRECTORIES} ${LJ_ENGINE_DIR}/LJ_LibUnitTest )

SET( CMAKE_VERBOSE_MAKEFILE OFF )

# list all source files here
SET( EXTERNAL_TESTS LJ_unittest_str.c LJ_unittest_mem.c LJ_unittest_math.c LJ_unittest_filesystem.c )

ADD_LIBRARY( LJ_LibUnitTest STATIC LJ_unittest.c LJ_unittest_tests.c LJ_unittest_register.c ${EXTERNAL_TESTS} )

