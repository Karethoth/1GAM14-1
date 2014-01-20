#pragma once
#ifndef _CHARACTER_HH_
#define _CHARACTER_HH_

#include "entity.hh"


class Character : public Node, Physical
{
 public:
	Character( std::string characterName="UnnamedCharacter" );
	virtual ~Character();

	virtual void Render();
	virtual void Update( double deltaTime );
	virtual void UpdateWorldInfo();

	virtual bool CollidesWith( const AABB& otherCollisionBox ) const;

	void SetCollisionBox( AABB newCollisionBox );
	virtual const AABB& GetCollisionBox() const;

	void SetVelocity( glm::vec3 newVelocity );
	glm::vec3 GetVelocity() const;


 protected:
	AABB collisionBox;


 private:
	std::vector<std::shared_ptr<Entity>> bodyParts;
	glm::vec3 velocity;
};

#endif

