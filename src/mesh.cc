#include "mesh.hh"


Mesh::Mesh()
{
	Node();

	SetLocation( glm::vec3( 0.0, 0.0, 0.0 ) );
}


Mesh::~Mesh()
{
	vboBuffer.clear();
	indexBuffer.clear();
}
