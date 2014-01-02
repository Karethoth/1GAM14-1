#pragma once
#ifndef _SHADER_HH_
#define _SHADER_HH_

#ifdef _WIN32
#include <Windows.h>
#endif


#include <string>
#include <exception>

#define GLEW_STATIC
#include <GL/glew.h>


class Shader
{
 public:
	Shader( GLenum type );
	~Shader();

	bool Load( const std::string &source );
	bool LoadFromFile( const std::string &path );
	const GLuint Get() const;


 private:
	GLenum type;
	GLuint shader;
	GLint  compiled;
};

#endif
