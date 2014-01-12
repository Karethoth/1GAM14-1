#pragma once
#ifndef _CHARACTER_HH_
#define _CHARACTER_HH_

#include "entity.hh"
#include <map>


class Character : public Node
{
 public:
	Character( std::string characterName="UnnamedCharacter" );
	virtual ~Character();

	virtual void Render();
	virtual void UpdateWorldInfo();


 private:
	std::map<std::string, std::shared_ptr<Entity>> bodyParts;
};

#endif
