#include "collisions.hh"


sAABB::sAABB( const glm::vec3& minimum, const glm::vec3& maximum ) : min( minimum ), max( maximum )
{
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

