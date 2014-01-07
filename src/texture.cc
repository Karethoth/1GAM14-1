#include "texture.hh"
#include "managers.hh"
#include <iostream>


Texture::Texture( std::string textureName )
{
	name = textureName;
	glGenTextures( 1, &textureId );
}


Texture::~Texture()
{
	if( textureId )
	{
		glDeleteTextures( 1, &textureId );
	}
}

