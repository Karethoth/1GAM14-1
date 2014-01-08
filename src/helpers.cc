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
	return a.x * b.y - a.y * b.x;;
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
	float tmpCross = Cross( bOffset, dOffset );

	float t = Cross( (c-a), dOffset / tmpCross );
	float u = Cross( (c-a), bOffset / tmpCross );

	// Do we have an intersection?
	if( ( tmpCross != 0 ) &&
		( t >= 0.0 && t <= 1.0 ) &&
		( u >= 0.0 && u <= 1.0 ) )
	{
		glm::vec2 point;
		return std::make_shared<glm::vec2>( a + t * bOffset );
	}
	return std::shared_ptr<glm::vec2>( nullptr );
}



float Noise2D( float x, float y )
{
	int n = (int)x + (int)y * 57;
	n = (n<<13) ^ n;
	return static_cast<float>( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0 );   
}


std::shared_ptr<std::vector<float>> NoiseArray( int width, int height,
                                                int startX, int startY )
{
	auto noiseArray = std::make_shared<std::vector<float>>();

	int stepX = 1;
	int stepY = 1;

	if( width < 0 )
	{
		stepX = -1;
	}

	if( height < 0 )
	{
		stepY = -1;
	}

	for( int y = 0; y < height; y += stepY )
	{
		for( int x = 0; x < width; x += stepX )
		{
			noiseArray->push_back(
				Noise2D( static_cast<float>( startX + x ),
				         static_cast<float>( startY + y ) )
			);
		}
	}

	return noiseArray;
}

