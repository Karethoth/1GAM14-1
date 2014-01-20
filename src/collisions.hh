#pragma once
#ifndef _COLLISIONS_HH_
#define _COLLISIONS_HH_

#include "../deb/glm/glm.hpp"
#include <vector>
#include <memory>


typedef struct sAABB
{
	glm::vec3 min;
	glm::vec3 max;

	sAABB( const glm::vec3& minimum = glm::vec3( 0.f ),
	       const glm::vec3& maximum = glm::vec3( 0.f ) );

	sAABB operator +( const glm::vec3& translation ) const;
	sAABB operator -( const glm::vec3& translation ) const;
	sAABB operator *( const glm::vec3& scale ) const;
} AABB;


bool AABBToAABBCollision( const AABB& a, const AABB& b );


struct sPhysical;
typedef std::vector<std::shared_ptr<sPhysical>> PhysicalList;

typedef struct sPhysical
{
	virtual bool CollidesWith( const AABB& otherCollisionBox ) const = 0;
	virtual AABB GetCollisionBox() const = 0;

	std::shared_ptr<PhysicalList> collidableObjects;
} Physical;


#endif

