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



struct Mesh
{
	Mesh( std::string name="UnnamedMesh" );
	virtual ~Mesh();

	bool GenerateGLBuffers();

	std::string name;
	GLuint vbo, ibo, vao;
	std::vector<VBOData> vertexBuffer;
	std::vector<unsigned int> indexBuffer;
};

#endif

