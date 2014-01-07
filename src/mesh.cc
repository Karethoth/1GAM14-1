#include "mesh.hh"
#include "managers.hh"
#include <iostream>


Mesh::Mesh( std::string meshName )
{
	name = meshName;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glGenBuffers( 1, &vbo );
	glGenBuffers( 1, &ibo );
}


Mesh::~Mesh()
{
	if( vao )
	{
		glDeleteVertexArrays( 1, &vao );
	}
	if( vbo )
	{
		glDeleteBuffers( 1, &vbo );
	}
	if( ibo )
	{
		glDeleteBuffers( 1, &ibo );
	}

	vertexBuffer.clear();
	indexBuffer.clear();
}



bool Mesh::GenerateGLBuffers( const std::string& shaderName )
{
	auto shaderProgram = shaderManager.Get( shaderName );
	glBindVertexArray( vao );

	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER,
	              vertexBuffer.size() * sizeof( VBOData ),
				  &vertexBuffer[0],
				  GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,
	              indexBuffer.size() * sizeof( GLfloat ),
				  &indexBuffer[0],
				  GL_STATIC_DRAW );

	glEnableVertexAttribArray( shaderProgram->GetAttribute( "vertexPosition" ) );
	glEnableVertexAttribArray( shaderProgram->GetAttribute( "vertexNormal" ) );
	glEnableVertexAttribArray( shaderProgram->GetAttribute( "textureCoord" ) );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );

	// Vertex position
	glVertexAttribPointer(
	   shaderProgram->GetAttribute( "vertexPosition" ),
	   3,
	   GL_FLOAT,
	   GL_FALSE,
	   sizeof( VBOData ),
	   (void*)0
	);

	// Vertex normal
	glVertexAttribPointer(
	   shaderProgram->GetAttribute( "vertexNormal" ),
	   3,
	   GL_FLOAT,
	   GL_TRUE,
	   sizeof( VBOData ),
	   (void*)sizeof(glm::vec3)
	);

	// TextureCoord
	glVertexAttribPointer(
	   shaderProgram->GetAttribute( "textureCoord" ),
	   2,
	   GL_FLOAT,
	   GL_FALSE,
	   sizeof( VBOData ),
	   (void*)(3*sizeof(glm::vec3))
	);

	glBindVertexArray( 0 );
	return true;
}

