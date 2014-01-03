#pragma once
#ifndef _MESH_HH_
#define _MESH_HH_

#include "node.hh"
#include <vector>


struct VBOData
{
	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 textCoord;
};



struct Mesh : public Node
{
	Mesh();
	~Mesh();

	std::vector<VBOData> vboBuffer;
	std::vector<unsigned int> indexBuffer;
};

#endif
