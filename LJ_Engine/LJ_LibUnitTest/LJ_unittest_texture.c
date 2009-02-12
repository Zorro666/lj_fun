#include "LJ.h"
#include "LJ_unittest.h"
#include "LJ_LibCommon/LJ_LibCommon.h"
#include "LJ_LibGraphics/LJ_LibGraphics.h"

#if LJ_USE_UNITTEST

#define LJ_TEST_TGA_BASE_NAME	("../test/test.tga")
#define LJ_TEST_TGA_WIDTH		(4)
#define LJ_TEST_TGA_HEIGHT		(8)

#define LJ_TEST_CHECK_IT( expr, msg ) \
	do \
	{ \
		if ( !(LJ_int)( (expr) ) ) \
		{ \
			LJ_outputPrintGold( msg ); \
			return LJ_FALSE; \
		} \
	} while ( 0 )

static LJ_bool LJ_testTGA( const LJ_char* const filename, const LJ_bool hFlip, const LJ_bool vFlip )
{
	LJ_fileHandle fh = LJ_FILE_HANDLE_INVALID;
	LJ_bool retVal = LJ_FALSE;
	LJ_uint8 temp8;
	LJ_uint16 temp16;
	LJ_uint y;
	LJ_textureHandle th = LJ_TEXTURE_HANDLE_INVALID;
	LJ_uint8* imageDataPtr = LJ_NULL;
	LJ_uint* tgaDataPtr = LJ_NULL;
	LJ_uint testImage[LJ_TEST_TGA_HEIGHT][LJ_TEST_TGA_WIDTH];
	LJ_uint i;

	fh = LJ_filesystemOpen( filename, LJ_FILE_MODE_WRITE );
	LJ_TEST_CHECK_IT( ( fh != LJ_FILE_HANDLE_INVALID ), ( "Can't open file '%s'\n", filename ) );

    // LJ_uint8  		identsize;          	// size of ID field that follows 18 byte header (0 usually)
	temp8 = 0;
	retVal = LJ_filesystemWrite( fh, &temp8, 1 );
	LJ_TEST_CHECK_IT( retVal, ( "Failed to write identsize '%s'\n", filename ) );

    //LJ_uint8  		colourmaptype;      	// type of colour map 0=none, 1=has palette
	temp8 = 0;
	retVal = LJ_filesystemWrite( fh, &temp8, 1 );
	LJ_TEST_CHECK_IT( retVal, ( "Failed to write colourmaptype '%s'\n", filename ) );

    //LJ_uint8  		imagetype;          	// type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed
	temp8 = 2;
	retVal = LJ_filesystemWrite( fh, &temp8, 1 );
	LJ_TEST_CHECK_IT( retVal, ( "Failed to write imagetype '%s'\n", filename ) );

    //LJ_uint16		colourmapstart;     	// first colour map entry in palette
	temp16 = 0;
	retVal = LJ_filesystemWrite( fh, &temp16, 2 );
	LJ_TEST_CHECK_IT( retVal, ( "Failed to write colourmapstart '%s'\n", filename ) );

    //LJ_uint16		colourmaplength;    	// number of colours in palette
	temp16 = 0;
	retVal = LJ_filesystemWrite( fh, &temp16, 2 );
	LJ_TEST_CHECK_IT( retVal, ( "Failed to write colourmaplength '%s'\n", filename ) );

    //LJ_uint8  		colourmapbits;      	// number of bits per palette entry 15,16,24,32
	temp8 = 0;
	retVal = LJ_filesystemWrite( fh, &temp8, 1 );
	LJ_TEST_CHECK_IT( retVal, ( "Failed to write colourmapbits '%s'\n", filename ) );

    //LJ_uint16		xstart;             	// image x origin
	temp16 = 0;
	retVal = LJ_filesystemWrite( fh, &temp16, 2 );
	LJ_TEST_CHECK_IT( retVal, ( "Failed to write xstart '%s'\n", filename ) );

    //LJ_uint16		ystart;             	// image y origin
	temp16 = 0;
	retVal = LJ_filesystemWrite( fh, &temp16, 2 );
	LJ_TEST_CHECK_IT( retVal, ( "Failed to write yorigin '%s'\n", filename ) );

    //LJ_uint16		width;              	// image width in pixels
	temp16 = LJ_TEST_TGA_WIDTH;
	retVal = LJ_filesystemWrite( fh, &temp16, 2 );
	LJ_TEST_CHECK_IT( retVal, ( "Failed to write width '%s'\n", filename ) );

    //LJ_uint16		height;             	// image height in pixels
	temp16 = LJ_TEST_TGA_HEIGHT;
	retVal = LJ_filesystemWrite( fh, &temp16, 2 );
	LJ_TEST_CHECK_IT( retVal, ( "Failed to write height '%s'\n", filename ) );

    //LJ_uint8  		bits;               	// image bits per pixel 8,16,24,32
	temp8 = 32;
	retVal = LJ_filesystemWrite( fh, &temp8, 1 );
	LJ_TEST_CHECK_IT( retVal, ( "Failed to write bits '%s'\n", filename ) );

    //LJ_uint8  		descriptor;         	// image descriptor bits (vh flip bits)
	temp8 = 0;
	// Set bit-4 to flip horizontally
	temp8 |= ( hFlip ) ? ( 1 << 4 ) : 0;
	// Set bit-5 to flip vertically
	temp8 |= ( vFlip ) ? ( 1 << 5 ) : 0;
	retVal = LJ_filesystemWrite( fh, &temp8, 1 );
	LJ_TEST_CHECK_IT( retVal, ( "Failed to write descriptor '%s'\n", filename ) );
    
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
			const LJ_uint red = alpha;

			pixel &= 0x00FFFFFF;
			pixel |= ( alpha << 24 );
			pixel |= ( red << 16 );
			testImage[y][x] = pixel;
		}
	}
	// TGA's are upside down with bottom of image at first bytes in the file
	for ( y = 0; y < LJ_TEST_TGA_HEIGHT; y++ )
	{
		LJ_uint x;
		const LJ_uint row = ( vFlip ) ? LJ_TEST_TGA_HEIGHT - y - 1 : y;
		LJ_uint8* const rowDataPtr = (LJ_uint8* const)( testImage[row] );
		
		// Write the data out pixel at a time and byte at a time
		i = 0;
		for ( x = 0; x < LJ_TEST_TGA_WIDTH; x++ )
		{
			// TGA pixel format is: ARGB which in the file is B G R A
			// Blue
			temp8 = rowDataPtr[i];
			i++;
			retVal = LJ_filesystemWrite( fh, &temp8, 1 );
			LJ_TEST_CHECK_IT( retVal, ( "Failed to write blue pixel %dx%d '%s'\n", x, y, filename ) );

			// Green
			temp8 = rowDataPtr[i];
			i++;
			retVal = LJ_filesystemWrite( fh, &temp8, 1 );
			LJ_TEST_CHECK_IT( retVal, ( "Failed to write green pixel %dx%d '%s'\n", x, y, filename ) );

			// Red
			temp8 = rowDataPtr[i];
			i++;
			retVal = LJ_filesystemWrite( fh, &temp8, 1 );
			LJ_TEST_CHECK_IT( retVal, ( "Failed to write red pixel %dx%d '%s'\n", x, y, filename ) );

			// Alpha
			temp8 = rowDataPtr[i];
			i++;
			retVal = LJ_filesystemWrite( fh, &temp8, 1 );
			LJ_TEST_CHECK_IT( retVal, ( "Failed to write alpha pixel %dx%d '%s'\n", x, y, filename ) );
		}
	}

	retVal = LJ_filesystemClose( fh );
	LJ_TEST_CHECK_IT( retVal, ( "Failed to close file '%s'\n", filename ) );

	th = LJ_textureLoadTGA( filename, &imageDataPtr );

	// Now compare the pixels
	tgaDataPtr = (LJ_uint*)( imageDataPtr );
	i = 0;
	for ( y = 0; y < LJ_TEST_TGA_HEIGHT; y++ )
	{
		LJ_uint x;
		
		// Write the data out pixel at a time and byte at a time
		for ( x = 0; x < LJ_TEST_TGA_WIDTH; x++ )
		{
			const LJ_uint tgaPixel = tgaDataPtr[i];
			const LJ_uint myPixel = testImage[y][x];
			// Convert tga pixel (BGRA) into ARGB pixel
			LJ_uint rgbaPixel = ( ( ( tgaPixel >> 24 ) & 0xFF ) << 24 ) | 
								( ( ( tgaPixel >> 0 ) & 0xFF ) << 16 ) |
								( ( ( tgaPixel >> 8 ) & 0xFF ) << 8 ) |
								( ( ( tgaPixel >> 16 ) & 0xFF ) << 0 );
			LJ_TEST_CHECK_IT( ( myPixel == rgbaPixel ), 
							  ( "Pixels don't match Expected 0x%X Got 0x%X file '%s'\n", myPixel, rgbaPixel, filename ) );
			i++;
		}
	}

	LJ_MEM_FREE( imageDataPtr );

	return LJ_TRUE;
}

LJ_UNITTEST_FUNCTION_START( texture, tga )
{
	// Write out a tga and read it back in
	const LJ_char* const filename = LJ_TEST_TGA_BASE_NAME;
	const LJ_bool vFlip = LJ_FALSE;
	const LJ_bool hFlip = LJ_FALSE;
	LJ_UNITTEST_TRUE( LJ_testTGA( filename, hFlip, vFlip ) );
}
LJ_UNITTEST_FUNCTION_END( texture, tga )

#endif // #if LJ_USE_UNITTEST


