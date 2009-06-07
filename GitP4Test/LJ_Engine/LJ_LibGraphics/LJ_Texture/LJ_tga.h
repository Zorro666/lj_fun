#ifndef LJ_TGA_H
#define LJ_TGA_H

// internal dependencies
#include "LJ_LibCommon/LJ_Base/LJ_types.h"

typedef struct LJ_TGA_Header
{
    LJ_uint8  		identsize;          	// size of ID field that follows 18 byte header (0 usually)
    LJ_uint8  		colourmaptype;      	// type of colour map 0=none, 1=has palette
    LJ_uint8  		imagetype;          	// type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed

    LJ_uint16		colourmapstart;     	// first colour map entry in palette
    LJ_uint16		colourmaplength;    	// number of colours in palette
    LJ_uint8  		colourmapbits;      	// number of bits per palette entry 15,16,24,32

    LJ_uint16		xstart;             	// image x origin
    LJ_uint16		ystart;             	// image y origin
    LJ_uint16		width;              	// image width in pixels
    LJ_uint16		height;             	// image height in pixels
    LJ_uint8  		bits;               	// image bits per pixel 8,16,24,32
    LJ_uint8  		descriptor;         	// image descriptor bits (xxvhxxx flip bits) - bit-4 h-flip, bit-5 v-flip
    
    // pixel data follows header
	// TGA pixel format is: ARGB
	// TGA's are upside down with bottom of image at first bytes in the file
    
} LJ_TGA_Header;

#endif // #ifndef LJ_TGA_H

