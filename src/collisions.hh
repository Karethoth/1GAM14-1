#pragma once
#ifndef _COLLISIONS_HH_
#define _COLLISIONS_HH_

#include "../deb/glm/glm.hpp"


typedef struct sAABB
{
	glm::vec3 min;
	glm::vec3 max;

	sAABB( const glm::vec3& minimum = glm::vec3( 0.f ),
	       const glm::vec3& maximum = glm::vec3( 0.f ) );

	sAABB operator +( const glm::vec3& translation );
	sAABB operator -( const glm::vec3& translation );
	sAABB operator *( const glm::vec3& scale );
} AABB;


bool AABBToAABBCollision( const AABB& a, const AABB& b );



typedef struct sPhysical
{
	virtual bool CollidesWith( const AABB& otherCollisionBox ) = 0;
} Physical;


#endif

