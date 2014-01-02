#pragma once
#ifndef _SHADERPROGRAM_HH_
#define _SHADERPROGRAM_HH_

#include "shader.hh"


class ShaderProgram
{
 public:
	ShaderProgram();
	~ShaderProgram();


	bool Load( const Shader &vertexShader,
	           const Shader &fragmentShader );

	const GLuint Get() const;


 private:
	GLuint program;
	GLint linked;
};

#endif
