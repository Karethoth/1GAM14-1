#pragma once
#ifndef _MESH_HH_
#define _MESH_HH_

#include "node.hh"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

	bool GenerateGLBuffers();

	GLuint vbo, ibo;
	std::vector<VBOData> vertexBuffer;
	std::vector<unsigned int> indexBuffer;
};

#endif
