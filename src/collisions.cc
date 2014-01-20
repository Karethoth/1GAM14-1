#include "collisions.hh"

inline float Min( const float& a, const float& b )
{
	return a <= b ? a : b;
}


inline float Max( const float& a, const float& b )
{
	return a >= b ? a : b;
}



sAABB::sAABB( const glm::vec3& minimum, const glm::vec3& maximum )
{
	min = glm::vec3(
		Min( minimum.x, maximum.x ),
		Min( minimum.y, maximum.y ),
		Min( minimum.z, maximum.z )
	);
	max = glm::vec3(
		Max( minimum.x, maximum.x ),
		Max( minimum.y, maximum.y ),
		Max( minimum.z, maximum.z )
	);
}


sAABB AABB::operator + ( const glm::vec3& translation ) const
{
	return( AABB( min+translation, max+translation ) );
}


sAABB AABB::operator - ( const glm::vec3& translation ) const
{
	return( AABB( min-translation, max-translation ) );
}


sAABB AABB::operator * ( const glm::vec3& scale ) const
{
	return( AABB( min*scale, max*scale ) );
}



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

