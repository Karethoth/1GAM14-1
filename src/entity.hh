#pragma once
#ifndef _ENTITY_HH_
#define _ENTITY_HH_

#include "node.hh"
#include "collisions.hh"


class Entity : public Node, Physical
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

	void SetCollisionBox( AABB newCollisionBox );

	virtual void Render();

	virtual bool CollidesWith( const AABB& otherCollisionBox ) const;
	virtual const AABB& GetCollisionBox() const;


 protected:
	AABB collisionBox;


 private:
	std::string meshName;
	std::string textureName;
	std::string shaderName;
};

#endif

