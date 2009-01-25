# add definitions, compiler switches, etc.
ADD_DEFINITIONS( ${GLOBAL_DEFINITIONS} )
INCLUDE_DIRECTORIES( ${GLOBAL_INCLUDE_DIRECTORIES} ${LJ_ENGINE_DIR}/LJ_LibCommon )

SET( CMAKE_VERBOSE_MAKEFILE OFF )

# list all source files here
SET( LJ_MEM_SRC LJ_Mem/LJ_mem.c LJ_Mem/LJ_memAlloc.c LJ_Mem/LJ_memDebug.c )
SET( LJ_BASE_SRC LJ_Base/LJ_types.c LJ_Base/LJ_str.c LJ_Base/LJ_output.c LJ_Base/LJ_assert.c )
SET( LJ_MATH_SRC LJ_Math/LJ_math.c )

ADD_LIBRARY( LJ_LibCommon STATIC ${LJ_BASE_SRC} ${LJ_MATH_SRC} ${LJ_MEM_SRC} )

