#include "textureManager.hh"
#include <png.h>
#include <iostream>


TextureManager::~TextureManager()
{
}



bool TextureManager::Load( const std::string& filepath )
{
	return Load( filepath, filepath );
}


bool TextureManager::Load( const std::string& filepath,
                           const std::string& key )
{
	png_infop   info_ptr;
	png_structp png_ptr;

	FILE *file;

	// Ugly, I know.
#ifdef _WIN32
	errno_t err = fopen_s( &file, filepath.c_str(), "rb" );
	if( err )
	{
#else
	file = fopen( filepath.c_str(), "rb" );
	if( !file )
	{
#endif
		return false;
	}

	auto fp = std::shared_ptr<FILE>( file, std::fclose  );

	png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );

	if( !png_ptr )
	{
		return false;
	}

	info_ptr = png_create_info_struct( png_ptr );
	if( info_ptr == NULL )
	{
		png_destroy_read_struct( &png_ptr, NULL, NULL );
		return false;
	}

	if( setjmp( png_jmpbuf( png_ptr ) ) )
	{
		png_destroy_read_struct( &png_ptr, &info_ptr, NULL );
		return false;
	}

	png_init_io( png_ptr, fp.get() );

	png_read_png( png_ptr, info_ptr, 0, NULL );

	png_uint_32 width = png_get_image_width( png_ptr, info_ptr );
	png_uint_32 height = png_get_image_height( png_ptr, info_ptr );

	png_uint_32 channels  = png_get_channels( png_ptr, info_ptr );
	png_uint_32 colorType = png_get_color_type( png_ptr, info_ptr );


	png_bytepp rowPointers;
	rowPointers = png_get_rows( png_ptr, info_ptr );

	auto data = new unsigned int[width*height];

	for( unsigned int y = 0; y < height; ++y )
	{
		memcpy( &data[y*width], rowPointers[y], width*channels );
	}

	png_destroy_read_struct( &png_ptr, &info_ptr, NULL );


	// Generate the OpenGL texture
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	auto texture = std::make_shared<Texture>( filepath );

	glBindTexture( GL_TEXTURE_2D, texture->textureId );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
					colorType == PNG_COLOR_TYPE_RGBA? GL_RGBA : GL_RGB,
					GL_UNSIGNED_BYTE, data );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	texture->name = key;
	Add( key, texture );

	delete[] data;
	return true;
};

