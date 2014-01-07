#pragma once
#ifndef _TEXTURE_HH_
#define _TEXTURE_HH_

#include "node.hh"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>


struct Texture
{
	Texture( std::string name="UnnamedTexture" );
	virtual ~Texture();

	std::string name;
	GLuint textureId;
};

#endif

