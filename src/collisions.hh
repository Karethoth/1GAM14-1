#pragma once
#ifndef _COLLISIONS_HH_
#define _COLLISIONS_HH_

#include "../deb/glm/glm.hpp"


typedef struct sAABB
{
	glm::vec3 min;
	glm::vec3 max;
} AABB;


bool AABBToAABBCollision( const AABB& a, const AABB& b );


#endif

