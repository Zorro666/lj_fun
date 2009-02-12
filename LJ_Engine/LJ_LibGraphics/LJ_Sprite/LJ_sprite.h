#ifndef LJ_SPRITE_HH
#define LJ_SPRITE_HH

#include "LJ_LibCommon/LJ_LibCommon.h"
#include "LJ_LibGraphics/LJ_Texture/LJ_texture.h"

typedef enum LJ_spriteLayerEnum
{
	LJ_SPRITE_BG,
	LJ_SPRITE_MAIN,
	LJ_SPRITE_FG
} LJ_spriteLayerEnum;

typedef struct LJ_sprite
{
	LJ_spriteLayerEnum layer;
	LJ_int priority;
	LJ_textureHandle texHandle;
	LJ_uint rgba;
	LJ_float x;
	LJ_float y;
	LJ_float width;
	LJ_float height;
	LJ_float u0;
	LJ_float v0;
	LJ_float u1;
	LJ_float v1;
} LJ_sprite;

void LJ_spriteRender( const LJ_sprite* const sprite );

#endif // #ifndef LJ_SPRITE_HH
