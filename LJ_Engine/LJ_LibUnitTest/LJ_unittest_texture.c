#include "LJ.h"
#include "LJ_unittest.h"
#include "LJ_LibCommon/LJ_LibCommon.h"
#include "LJ_LibGraphics/LJ_LibGraphics.h"

#if LJ_USE_UNITTEST

#define LJ_TEST_TGA_NAME	("../test/test.tga")
#define LJ_TEST_TGA_WIDTH	(4)
#define LJ_TEST_TGA_HEIGHT	(8)

LJ_UNITTEST_FUNCTION_START( texture, tga )
{
	// Manually write out a TGA file (verified by an external application)
	// Then read it in
	LJ_fileHandle fh = LJ_FILE_HANDLE_INVALID;
	LJ_bool retVal = LJ_FALSE;
	LJ_uint8 temp8;
	LJ_uint16 temp16;
	LJ_uint y;
	LJ_textureHandle th = LJ_TEXTURE_HANDLE_INVALID;
	LJ_uint8* imageDataPtr = LJ_NULL;
	LJ_uint testImage[LJ_TEST_TGA_HEIGHT][LJ_TEST_TGA_WIDTH];

	fh = LJ_filesystemOpen( LJ_TEST_TGA_NAME, LJ_FILE_MODE_WRITE );
	LJ_UNITTEST_TRUE( ( fh != LJ_FILE_HANDLE_INVALID ) );

    // LJ_uint8  		identsize;          	// size of ID field that follows 18 byte header (0 usually)
	temp8 = 0;
	retVal = LJ_filesystemWrite( fh, &temp8, 1 );
	LJ_UNITTEST_TRUE( retVal );

    //LJ_uint8  		colourmaptype;      	// type of colour map 0=none, 1=has palette
	temp8 = 0;
	retVal = LJ_filesystemWrite( fh, &temp8, 1 );
	LJ_UNITTEST_TRUE( retVal );

    //LJ_uint8  		imagetype;          	// type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed
	temp8 = 2;
	retVal = LJ_filesystemWrite( fh, &temp8, 1 );
	LJ_UNITTEST_TRUE( retVal );

    //LJ_uint16		colourmapstart;     	// first colour map entry in palette
	temp16 = 0;
	retVal = LJ_filesystemWrite( fh, &temp16, 2 );
	LJ_UNITTEST_TRUE( retVal );

    //LJ_uint16		colourmaplength;    	// number of colours in palette
	temp16 = 0;
	retVal = LJ_filesystemWrite( fh, &temp16, 2 );
	LJ_UNITTEST_TRUE( retVal );

    //LJ_uint8  		colourmapbits;      	// number of bits per palette entry 15,16,24,32
	temp8 = 0;
	retVal = LJ_filesystemWrite( fh, &temp8, 1 );
	LJ_UNITTEST_TRUE( retVal );

    //LJ_uint16		xstart;             	// image x origin
	temp16 = 0;
	retVal = LJ_filesystemWrite( fh, &temp16, 2 );
	LJ_UNITTEST_TRUE( retVal );

    //LJ_uint16		ystart;             	// image y origin
	temp16 = 0;
	retVal = LJ_filesystemWrite( fh, &temp16, 2 );
	LJ_UNITTEST_TRUE( retVal );

    //LJ_uint16		width;              	// image width in pixels
	temp16 = LJ_TEST_TGA_WIDTH;
	retVal = LJ_filesystemWrite( fh, &temp16, 2 );
	LJ_UNITTEST_TRUE( retVal );

    //LJ_uint16		height;             	// image height in pixels
	temp16 = LJ_TEST_TGA_HEIGHT;
	retVal = LJ_filesystemWrite( fh, &temp16, 2 );
	LJ_UNITTEST_TRUE( retVal );

    //LJ_uint8  		bits;               	// image bits per pixel 8,16,24,32
	temp8 = 32;
	retVal = LJ_filesystemWrite( fh, &temp8, 1 );
	LJ_UNITTEST_TRUE( retVal );

    //LJ_uint8  		descriptor;         	// image descriptor bits (vh flip bits)
	temp8 = 0;
	retVal = LJ_filesystemWrite( fh, &temp8, 1 );
	LJ_UNITTEST_TRUE( retVal );
    
    // pixel data follows header
	for ( y = 0; y < LJ_TEST_TGA_HEIGHT; y++ )
	{
		LJ_uint x;
		for ( x = 0; x < LJ_TEST_TGA_WIDTH; x++ )
		{
			// TGA pixel format is: ARGB
			LJ_uint colours[4] = { 0xFF000000, 0x80FF0000, 0x8000FF00, 0xFF0000FF };
			LJ_uint pixel = colours[x];
			const LJ_uint alpha = ( ( ( 1 << y ) << 1 ) - 1 );

			pixel &= 0x00FFFFFF;
			pixel |= ( alpha << 24 );
			testImage[y][x] = pixel;
		}
	}
	// TGA's are upside down with bottom of image at first bytes in the file
	for ( y = 0; y < LJ_TEST_TGA_HEIGHT; y++ )
	{
		LJ_uint* const rowDataPtr = testImage[LJ_TEST_TGA_HEIGHT - y - 1];
		retVal = LJ_filesystemWrite( fh, rowDataPtr, LJ_TEST_TGA_WIDTH * 4 );
		LJ_UNITTEST_TRUE( retVal );
	}

	retVal = LJ_filesystemClose( fh );
	LJ_UNITTEST_TRUE( retVal );

	th = LJ_textureLoadTGA( LJ_TEST_TGA_NAME, &imageDataPtr );

	// Now compare the pixels
	LJ_UNITTEST_TRUE( LJ_memIsSame( imageDataPtr, testImage, LJ_TEST_TGA_HEIGHT * LJ_TEST_TGA_WIDTH * 4 ) );

	LJ_UNITTEST_TRUE( LJ_FALSE );

	LJ_MEM_FREE( imageDataPtr );
}
LJ_UNITTEST_FUNCTION_END( texture, tga )

#endif // #if LJ_USE_UNITTEST


