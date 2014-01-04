#pragma once
#ifndef _SHADERPROGRAM_HH_
#define _SHADERPROGRAM_HH_

#include "shader.hh"
#include <map>


class ShaderProgram
{
 public:
	ShaderProgram();
	~ShaderProgram();


	bool Load( const Shader &vertexShader,
	           const Shader &fragmentShader );

	const GLuint Get() const;

	const GLint GetUniform( const std::string &uniformName );


 private:
	GLuint program;
	GLint linked;

	std::map<std::string, GLint> uniforms;
};

#endif
