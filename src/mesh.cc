#include "mesh.hh"


Mesh::Mesh()
{
	Node();

	glGenBuffers( 1, &vbo );
	glGenBuffers( 1, &ibo );
}


Mesh::~Mesh()
{
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



bool Mesh::GenerateGLBuffers()
{
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER,
	              vertexBuffer.size() * sizeof VBOData,
				  &vertexBuffer[0],
				  GL_STATIC_DRAW );


	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER,
	              indexBuffer.size() * sizeof VBOData,
				  &indexBuffer[0],
				  GL_STATIC_DRAW );

	return true;
}
