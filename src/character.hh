#pragma once
#ifndef _CHARACTER_HH_
#define _CHARACTER_HH_

#include "entity.hh"


class Character : public Node
{
 public:
	Character( std::string characterName="UnnamedCharacter" );
	virtual ~Character();

	virtual void Render();
	virtual void Update( double deltaTime );
	virtual void UpdateWorldInfo();

	void SetVelocity( glm::vec3 newVelocity );
	glm::vec3 GetVelocity() const;


 private:
	std::vector<std::shared_ptr<Entity>> bodyParts;
	glm::vec3 velocity;
};

#endif

