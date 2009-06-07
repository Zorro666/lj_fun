# add definitions, compiler switches, etc.
ADD_DEFINITIONS( ${GLOBAL_DEFINITIONS} )
INCLUDE_DIRECTORIES( ${GLOBAL_INCLUDE_DIRECTORIES} ${LJ_ENGINE_DIR}/LJ_LibGraphics )

SET( CMAKE_VERBOSE_MAKEFILE OFF )

# list all source files here
SET( SPRITE_SRC LJ_Sprite/LJ_sprite.c )
SET( TEXTURE_SRC LJ_Texture/LJ_texture.c )
ADD_LIBRARY( LJ_LibGraphics STATIC LJ_graphics.c ${TEXTURE_SRC} ${SPRITE_SRC} )

