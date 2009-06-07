#include "LJ.h"
#include "LJ_LibCommon/LJ_LibCommon.h"

#include "LJ_Texture/LJ_texture.h"
#include "LJ_Texture/LJ_tga.h"

#include <GL/gl.h>

#ifndef LJ_TEXTURE_MAX_NUM
#define LJ_TEXTURE_MAX_NUM		(512)
#endif // #ifndef LJ_TEXTURE_MAX_NUM

#ifndef LJ_TEXTURE_DEBUG
#if LJ_DEBUG
	#define LJ_TEXTURE_DEBUG	(1)
#endif // #if LJ_DEBUG
#endif // #ifndef LJ_TEXTURE_DEBUG

typedef struct LJ_textureInfo
{
	void* address;
	LJ_uint bindValue;
	LJ_uint width; 
	LJ_uint height; 
	// Need to include the texture format here as well
} LJ_textureInfo;

static LJ_textureInfo s_textureInfos[LJ_TEXTURE_MAX_NUM];
static LJ_uint s_textureMaxActiveInfo = 0;
static LJ_uint s_textureFirstFreeInfo = 0;

void LJ_textureInit( void )
{
	LJ_uint i;
	for ( i = 0; i < LJ_TEXTURE_MAX_NUM; i++ )
	{
		s_textureInfos[i].address = LJ_NULL;
		s_textureInfos[i].bindValue = 0;
		s_textureInfos[i].width = 0;
		s_textureInfos[i].height = 0;
	}
	s_textureFirstFreeInfo = 0;
	s_textureMaxActiveInfo = 0;
}

void LJ_textureReset( void )
{
}

void LJ_textureShutdown( void )
{
}

static LJ_textureInfo* LJ_textureGetInfo( const LJ_textureHandle th )
{
	const LJ_uint i = (LJ_uint)( th );
	LJ_textureInfo* const texInfo = s_textureInfos + ( i - 1 );
	return texInfo;
}

// Internal texture handle management functions
static LJ_textureHandle LJ_textureGetNewHandle( void )
{
	LJ_uint i;
	for ( i = s_textureFirstFreeInfo; i < LJ_TEXTURE_MAX_NUM; i++ )
	{
		LJ_textureInfo* const texInfo = s_textureInfos + i;
		if ( texInfo->bindValue == 0 )
		{
			const LJ_textureHandle texHandle = (LJ_textureHandle)( i + 1 );
			texInfo->bindValue = (LJ_uint)( texHandle );
			s_textureFirstFreeInfo++;

			if ( i > s_textureMaxActiveInfo )
			{
				s_textureMaxActiveInfo = i;
			}
			return texHandle;
		}
	}
	return LJ_TEXTURE_HANDLE_INVALID;
}

static LJ_bool LJ_textureFreeHandle( const LJ_textureHandle texHandle )
{
	LJ_textureInfo* const texInfo = LJ_textureGetInfo( texHandle );

	if ( texHandle == LJ_TEXTURE_HANDLE_INVALID )
	{
		return LJ_FALSE;
	}
	if ( texHandle >= LJ_TEXTURE_MAX_NUM )
	{
		return LJ_FALSE;
	}

	texInfo->address = LJ_NULL;
	texInfo->bindValue = 0;
	texInfo->width = 0;
	texInfo->height = 0;
	if ( texHandle < s_textureFirstFreeInfo )
	{
		s_textureFirstFreeInfo = texHandle;
	}
	if ( texHandle == s_textureMaxActiveInfo )
	{
		// This might not be the true maximum - could do a backwards search to find the first non-free alloc
		if ( s_textureMaxActiveInfo > 0 )
		{
			s_textureMaxActiveInfo--;
		}
	}

	return LJ_TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description: 
//
/////////////////////////////////////////////////////////////////////////////////////////////////

LJ_textureHandle LJ_textureLoadTGA( const char* const filename, LJ_uint8** imageDataPtr )
{
	LJ_textureHandle texHandle = LJ_TEXTURE_HANDLE_INVALID;
	const LJ_int fileSize = LJ_filesystemGetSize( filename );
	LJ_fileHandle fh = LJ_FILE_HANDLE_INVALID;
	LJ_TGA_Header header;
	LJ_bool retVal;
	LJ_uint32 rowSize;
	LJ_uint8* rowData;
	LJ_uint32 imageSize;
	LJ_uint8* imageData;
	LJ_uint y;
	LJ_textureInfo* texInfo;
	const LJ_uint lodLevel = 0;
	const LJ_uint borderSize = 0;
	LJ_bool vFlip = LJ_FALSE;
	LJ_bool hFlip = LJ_FALSE;

	if ( fileSize < 0 )
	{
		return LJ_TEXTURE_HANDLE_INVALID;
	}

	fh = LJ_filesystemOpen( filename, LJ_FILE_MODE_READ );
	if ( fh == LJ_FILE_HANDLE_INVALID )
	{
		LJ_assert( LJ_FALSE, ( "Failed to open TGA '%s'\n", filename ) );
		return LJ_TEXTURE_HANDLE_INVALID;
	}

	// Read in the header member by member because of mis-alignment on the short's

    // LJ_uint8  		identsize;          	// size of ID field that follows 18 byte header (0 usually)
	retVal = LJ_filesystemRead( fh, &header.identsize, sizeof( header.identsize ) );
	if ( retVal == LJ_FALSE )
	{
		LJ_assert( LJ_FALSE, ( "Failed to read identsize from TGA '%s'\n", filename ) );
		LJ_filesystemClose( fh );
		return LJ_TEXTURE_HANDLE_INVALID;
	}
    // LJ_uint8  		colourmaptype;      	// type of colour map 0=none, 1=has palette
	retVal = LJ_filesystemRead( fh, &header.colourmaptype, sizeof( header.colourmaptype ) );
	if ( retVal == LJ_FALSE )
	{
		LJ_assert( LJ_FALSE, ( "Failed to read colourmaptype from TGA '%s'\n", filename ) );
		LJ_filesystemClose( fh );
		return LJ_TEXTURE_HANDLE_INVALID;
	}
    // LJ_uint8  		imagetype;          	// type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed
	retVal = LJ_filesystemRead( fh, &header.imagetype, sizeof( header.imagetype ) );
	if ( retVal == LJ_FALSE )
	{
		LJ_assert( LJ_FALSE, ( "Failed to read imagetype from TGA '%s'\n", filename ) );
		LJ_filesystemClose( fh );
		return LJ_TEXTURE_HANDLE_INVALID;
	}
    // LJ_uint16		colourmapstart;     	// first colour map entry in palette
	retVal = LJ_filesystemRead( fh, &header.colourmapstart, sizeof( header.colourmapstart ) );
	if ( retVal == LJ_FALSE )
	{
		LJ_assert( LJ_FALSE, ( "Failed to read colourmapstart from TGA '%s'\n", filename ) );
		LJ_filesystemClose( fh );
		return LJ_TEXTURE_HANDLE_INVALID;
	}
    // LJ_uint16		colourmaplength;    	// number of colours in palette
	retVal = LJ_filesystemRead( fh, &header.colourmaplength, sizeof( header.colourmaplength ) );
	if ( retVal == LJ_FALSE )
	{
		LJ_assert( LJ_FALSE, ( "Failed to read colourmaplength from TGA '%s'\n", filename ) );
		LJ_filesystemClose( fh );
		return LJ_TEXTURE_HANDLE_INVALID;
	}
    // LJ_uint8  		colourmapbits;      	// number of bits per palette entry 15,16,24,32
	retVal = LJ_filesystemRead( fh, &header.colourmapbits, sizeof( header.colourmapbits ) );
	if ( retVal == LJ_FALSE )
	{
		LJ_assert( LJ_FALSE, ( "Failed to read colourmapbits from TGA '%s'\n", filename ) );
		LJ_filesystemClose( fh );
		return LJ_TEXTURE_HANDLE_INVALID;
	}

    // LJ_uint16		xstart;             	// image x origin
	retVal = LJ_filesystemRead( fh, &header.xstart, sizeof( header.xstart ) );
	if ( retVal == LJ_FALSE )
	{
		LJ_assert( LJ_FALSE, ( "Failed to read xstart from TGA '%s'\n", filename ) );
		LJ_filesystemClose( fh );
		return LJ_TEXTURE_HANDLE_INVALID;
	}
    // LJ_uint16		ystart;             	// image y origin
	retVal = LJ_filesystemRead( fh, &header.ystart, sizeof( header.ystart ) );
	if ( retVal == LJ_FALSE )
	{
		LJ_assert( LJ_FALSE, ( "Failed to read ystart from TGA '%s'\n", filename ) );
		LJ_filesystemClose( fh );
		return LJ_TEXTURE_HANDLE_INVALID;
	}
    // LJ_uint16		width;              	// image width in pixels
	retVal = LJ_filesystemRead( fh, &header.width, sizeof( header.width ) );
	if ( retVal == LJ_FALSE )
	{
		LJ_assert( LJ_FALSE, ( "Failed to read width from TGA '%s'\n", filename ) );
		LJ_filesystemClose( fh );
		return LJ_TEXTURE_HANDLE_INVALID;
	}
    // LJ_uint16		height;             	// image height in pixels
	retVal = LJ_filesystemRead( fh, &header.height, sizeof( header.height ) );
	if ( retVal == LJ_FALSE )
	{
		LJ_assert( LJ_FALSE, ( "Failed to read height from TGA '%s'\n", filename ) );
		LJ_filesystemClose( fh );
		return LJ_TEXTURE_HANDLE_INVALID;
	}
    // LJ_uint8  		bits;               	// image bits per pixel 8,16,24,32
	retVal = LJ_filesystemRead( fh, &header.bits, sizeof( header.bits ) );
	if ( retVal == LJ_FALSE )
	{
		LJ_assert( LJ_FALSE, ( "Failed to read bits from TGA '%s'\n", filename ) );
		LJ_filesystemClose( fh );
		return LJ_TEXTURE_HANDLE_INVALID;
	}
    // LJ_uint8  		descriptor;         	// image descriptor bits (xxvhxxx flip bits) - bit-4 h-flip, bit-5 v-flip
	retVal = LJ_filesystemRead( fh, &header.descriptor, sizeof( header.descriptor ) );
	if ( retVal == LJ_FALSE )
	{
		LJ_assert( LJ_FALSE, ( "Failed to read descriptor from TGA '%s'\n", filename ) );
		LJ_filesystemClose( fh );
		return LJ_TEXTURE_HANDLE_INVALID;
	}
	vFlip = ( header.descriptor & ( 1 << 5 ) );
	hFlip = ( header.descriptor & ( 1 << 4 ) );
	LJ_assert( ( hFlip == LJ_FALSE ), ( "horizontal flip not supported in TGA '%s'\n", filename ) );

    // colourmaptype = type of colour map 0=none, 1=has palette
	// Only support colourmaptype = 0 (no palette)
	if ( header.colourmaptype != 0 )
	{
		LJ_assert( LJ_FALSE, ( "Unsupported colourmaptype %d TGA '%s'\n", header.colourmaptype, filename ) );
		LJ_filesystemClose( fh );
		return LJ_TEXTURE_HANDLE_INVALID;
	}

    // imagetype = type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed
	// Only support imagetype = 2 (rgba)
	if ( header.imagetype != 2 )
	{
		LJ_assert( LJ_FALSE, ( "Unsupported imagetype %d TGA '%s'\n", header.imagetype, filename ) );
		LJ_filesystemClose( fh );
		return LJ_TEXTURE_HANDLE_INVALID;
	}

	// Only support bits = 24 (rgb) or 32 (rgba)
	if ( ( header.bits != 24 ) && ( header.bits != 32 ) )
	{
		LJ_assert( LJ_FALSE, ( "Unsupported bit-depth %d TGA '%s'\n", header.bits, filename ) );
		LJ_filesystemClose( fh );
		return LJ_TEXTURE_HANDLE_INVALID;
	}

	// Skip the ID data (identsize = byte size of ID data usually 0)
	if ( header.identsize > 0 )
	{
		LJ_uint8 dummy[256];
		retVal = LJ_filesystemRead( fh, dummy, header.identsize );
		if ( retVal == LJ_FALSE )
		{
			LJ_assert( LJ_FALSE, ( "Failed to read ID data %d bytes TGA '%s'\n", header.identsize, filename ) );
			LJ_filesystemClose( fh );
			return LJ_TEXTURE_HANDLE_INVALID;
		}
	}
	
	// Read in the image row by row
	rowSize = ( header.width * header.bits ) >> 3;
	rowData = LJ_MEM_ALLOC( LJ_memGetSystemAllocHandle(), LJ_uint8, rowSize );

	// Allocate the image memory
	imageSize = ( header.width * header.height ) * 4;
	imageData = LJ_MEM_ALLOC( LJ_memGetSystemAllocHandle(), LJ_uint8, imageSize );

	for ( y = 0; y < header.height; y++ )
	{
		LJ_uint x;
		LJ_uint i;
		LJ_uint j;
		const LJ_uint row = ( vFlip ) ? header.height - y - 1 : y;
		LJ_uint8* const rowStart = imageData + row * ( header.width * 4 );
		const LJ_bool readAlpha = ( header.bits == 32 ) ? LJ_TRUE : LJ_FALSE;

		retVal = LJ_filesystemRead( fh, rowData, rowSize );
		if ( retVal == LJ_FALSE )
		{
			LJ_filesystemClose( fh );
			return LJ_TEXTURE_HANDLE_INVALID;
		}
		i = 0;
		j = 0;
		for ( x = 0; x < header.width; x++ )
		{
			// TGA pixel format is: BGRA : byte by byte in the file is B G R A
			// Output pixel format is: RGBA : byte by byte is R G B A

			// TGA Blue data
			rowStart[i+2] = rowData[j++];
			// TGA Green data
			rowStart[i+1] = rowData[j++];
			// TGA Red data
			rowStart[i+0] = rowData[j++];

			// Only support 24-bit or 32-bit input images
			if ( readAlpha )
			{
				// Alpha from the input image
				rowStart[i+3] = rowData[j++];
			}
			else
			{
				// 24-bit image fix alpha to be 0xFF
				rowStart[i+3] = 0xFF;
			}
			i += 4;
		}
	}
	LJ_MEM_FREE( rowData );

	retVal = LJ_filesystemClose( fh );
	if ( retVal == LJ_FALSE )
	{
		LJ_assert( LJ_FALSE, ( "Failed to close file '%s'\n", filename ) );
		return LJ_TEXTURE_HANDLE_INVALID;
	}

	texHandle = LJ_textureGetNewHandle();
	LJ_assert( texHandle != LJ_TEXTURE_HANDLE_INVALID, ( "" ) );
	texInfo = LJ_textureGetInfo( texHandle );

	// Now put the image data into the hw texture e.g. GL, glBindTexture(), glTexImage2D()
	// Don't forget to set the min, mag filters to get mipmaps correct
	glBindTexture( GL_TEXTURE_2D, texInfo->bindValue );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, lodLevel, GL_RGBA, header.width, header.height, borderSize, GL_RGBA, GL_UNSIGNED_BYTE, imageData );

#if LJ_TEXTURE_DEBUG
	{
		LJ_uint8* newImageData;
		newImageData = LJ_MEM_ALLOC( LJ_memGetSystemAllocHandle(), LJ_uint8, imageSize );
		glGetTexImage( GL_TEXTURE_2D, lodLevel, GL_RGBA, GL_UNSIGNED_BYTE, newImageData );
		LJ_assert( ( LJ_memIsSame( imageData, newImageData, imageSize ) == LJ_TRUE ), ( "gl texture doesn't match texture data" ) );
		LJ_MEM_FREE( newImageData );
	}
#endif // #if LJ_TEXTURE_DEBUG

	// free the memory if the calling function doesn't want the image data (the default operation)
	if ( imageDataPtr == LJ_NULL )
	{
		LJ_MEM_FREE( imageData );
	}
	else
	{
		// Up to the calling function to release the memory
		*imageDataPtr = imageData;
	}

	// Fill in the info
	texInfo->width = header.width;
	texInfo->height = header.height;
	texInfo->address = imageDataPtr;

	return texHandle;
}

LJ_bool LJ_textureFreeTexture( const LJ_textureHandle texHandle )
{
	//LJ_textureInfo* const texInfo = s_textureInfos + texHandle;
	LJ_bool retVal;

	if ( texHandle == LJ_TEXTURE_HANDLE_INVALID )
	{
		return LJ_FALSE;
	}
	if ( texHandle >= LJ_TEXTURE_MAX_NUM )
	{
		return LJ_FALSE;
	}

	retVal = LJ_textureFreeHandle( texHandle );

	glDeleteTextures( 1, &texHandle );

	return retVal;
}

