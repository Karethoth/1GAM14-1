#include "helpers.hh"

const double piDivided = 3.141592653589793 / 180.0;


float  ToRadians( float degrees )
{
	return  degrees * static_cast<float>( piDivided );
}


double ToRadians( double degrees )
{
	return  degrees * piDivided;
}



float Cross( const glm::vec2& a, const glm::vec2& b )
{
	glm::vec3 v1( a.x, a.y, 0.0f );
	glm::vec3 v2( b.x, b.y, 0.0f );

	return ( v1, v2 ).z;
}


std::shared_ptr<glm::vec2> PointInBetween( const glm::vec2& a,
                                           const glm::vec2& b,
                                           double ratio )
{
	if( ratio > 1.0 )
		ratio = 1.0;
	else if( ratio < 0.0 )
		ratio = 0.0;

	glm::vec2 delta = b-a;
	delta *= ratio;

	return std::make_shared<glm::vec2>( delta + a );
}



std::shared_ptr<glm::vec2> Intersection( const glm::vec2& a,
                                         const glm::vec2& bOffset,
                                         const glm::vec2& c,
                                         const glm::vec2& dOffset )
{
	float t = Cross( (c-a), dOffset / ( Cross( bOffset, dOffset ) ) );
	float u = Cross( (c-a), bOffset / ( Cross( bOffset, dOffset ) ) );

	auto point = std::make_shared<glm::vec2>();
	return point;
}

