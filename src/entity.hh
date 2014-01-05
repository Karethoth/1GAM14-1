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

	virtual void Render();


 private:
	std::string meshName;
};

#endif

