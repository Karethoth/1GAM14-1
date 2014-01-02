#include "shader.hh"
#include <memory>
#include <iostream>


Shader::Shader( GLenum type ) : type(type)
{
	compiled = 0;
	shader = 0;
}



Shader::~Shader()
{
	glDeleteShader( shader );
}



bool Shader::Load( const std::string &source )
{
	if( compiled || shader )
	{
		std::cerr << "Error: Trying to use single Shader object to load multiple shaders!\n";
		return false;
	}

	shader = glCreateShader( type );
	if( !shader )
		return false;

	const GLchar *sourcePtr = (GLchar*)source.c_str();
	glShaderSource( shader, 1, &sourcePtr, NULL );

	glCompileShader( shader );
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );

	if( !compiled )
	{
		GLint infoLen = 0;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &infoLen );
		if( infoLen > 1 )
		{
			char* infoLog = new char[ sizeof( char )*infoLen ];
			std::string errorMsg;

			if( infoLog )
			{
				glGetShaderInfoLog( shader, infoLen, NULL, infoLog );
				errorMsg = "Error compiling shader: " + std::string( infoLog );
				delete[] infoLog;
			}
			else
			{
				errorMsg = "Error compiling shader and also failed to allocate memory for infolog!";
			}
			std::cerr << errorMsg << "\n";
		}

		glDeleteShader( shader );
		shader = 0;
		return false;
	}

	return true;
}



const GLuint Shader::Get() const
{
	return shader;
}
