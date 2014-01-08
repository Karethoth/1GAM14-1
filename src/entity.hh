#pragma once
#ifndef _ENTITY_HH_
#define _ENTITY_HH_

#include "node.hh"


class Entity : public Node
{
 public:
	Entity( std::string entityName="UnnamedEntity" );
	virtual ~Entity();

	void SetMeshName( std::string newMeshName );
	std::string GetMeshName();

	void SetTextureName( std::string newTextureName );
	std::string GetTextureName();

	void SetShaderName( std::string newShaderName );
	std::string GetShaderName();

	virtual void Render();


 private:
	std::string meshName;
	std::string textureName;
	std::string shaderName;
};

#endif

