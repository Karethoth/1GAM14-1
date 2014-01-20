#include "collisions.hh"


bool AABBToAABBCollision( const AABB& a, const AABB& b )
{
	return (
		a.max.x > b.min.x &&
		a.min.x < b.max.x &&
		a.max.y > b.min.y &&
		a.min.y < b.max.y &&
		a.max.z > b.min.z &&
		a.min.z < b.max.z
	);
}

