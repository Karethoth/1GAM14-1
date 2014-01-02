#include "shaderProgram.hh"
#include <iostream>


ShaderProgram::ShaderProgram()
{
	program = glCreateProgram();
	linked  = 0;
}



ShaderProgram::~ShaderProgram()
{
	if( program )
	{
		glDeleteProgram( program );

	}
}



bool ShaderProgram::Load( const Shader &vertexShader,
                          const Shader &fragmentShader )
{
	if( !program )
	{
		std::cerr << "Failed to create shader program.\n";
		return false;
	}

	if( linked )
	{
		std::cerr << "Error: Trying to use single ShaderProgram object to link multiple shaders!\n";
		return false;
	}


	glAttachShader( program, vertexShader.Get() );
	glAttachShader( program, fragmentShader.Get() );

	/* Binding could be done as a "callback" to a passed function. */
	glBindAttribLocation( program, 0, "vPosition" );

	glLinkProgram( program );
	glGetProgramiv( program, GL_LINK_STATUS, &linked );

	if( !linked )
	{
		GLint infoLen = 0;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLen );
		if( infoLen > 1 )
		{
			char* infoLog = new char[ sizeof( char )*infoLen ];
			std::string errorMsg;

			if( infoLog )
			{
				glGetProgramInfoLog( program, infoLen, NULL, infoLog );
				errorMsg = "Error linking shader program: " + std::string( infoLog );
				delete[] infoLog;
			}
			else
			{
				errorMsg = "Error linking shader program and also failed to allocate memory for infolog!";
			}
			std::cerr << errorMsg << "\n";
		}

		glDeleteProgram( program );
		program = 0;
		return false;
	}

	return true;
}


const GLuint ShaderProgram::Get() const
{
	return program;
}
