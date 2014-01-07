#include "surface.hh"
#include "helpers.hh"
#include <iostream>
using namespace glm;


Surface::Surface()
{
	Surface( 1.0, 1.0 );
}


Surface::Surface( float width, float height )
{
	size = vec2( width, height );
	textureCorners[0] = vec2( 0.0, 1.0 );
	textureCorners[1] = vec2( 1.0, 1.0 );
	textureCorners[2] = vec2( 0.0, 0.0 );
	textureCorners[3] = vec2( 1.0, 0.0 );
}



void Surface::SetTextureCorners( const vec2 corners[4]  )
{
	textureCorners[0] = corners[0];
	textureCorners[1] = corners[1];
	textureCorners[2] = corners[2];
	textureCorners[3] = corners[3];
}


// This method returns a subsurfaced surface as a mesh.
// ( The surface is split in pieces of wanted amount )
std::shared_ptr<Mesh> Surface::GenerateMesh( int xSplits, int ySplits )
{
	// TODO: Handle unsafe values, like negative splits
	auto mesh = std::make_shared<Mesh>();
	VBOData tmp;

	xSplits += 2;
	ySplits += 2;

	for( int y=0; y < ySplits; ++y )
	{
		for( int x=0; x < xSplits; ++x )
		{
			tmp.vertex = vec3( size.x/(xSplits-1)*x, 0, size.y/(ySplits-1)*y );
			tmp.normal = vec3( 0.0, 1.0, 0.0 );

			// Calculate the texture coordinate
			float ratioWidth  = (float)x/(xSplits-1);
			float ratioHeight = (float)y/(ySplits-1);
			auto middleAB = PointInBetween( textureCorners[0], textureCorners[1], ratioWidth );
			auto middleAC = PointInBetween( textureCorners[0], textureCorners[2], ratioHeight );
			auto middleCD = PointInBetween( textureCorners[2], textureCorners[3], ratioWidth );
			auto middleBD = PointInBetween( textureCorners[1], textureCorners[3], ratioHeight );

			// Find the intersection point
			auto textCoord = Intersection( *middleAB,
			                               *middleCD-*middleAB,
			                               *middleAC,
			                               *middleBD-*middleAC );
			if( textCoord )
			{
				tmp.textCoord = *textCoord;
			}

			mesh->vertexBuffer.push_back( tmp );
		}
	}

	// Create the index buffer
	int yLod = ySplits-1;
	int xLod = xSplits-1;

	for( int y=0; y < yLod; ++y )
	{
		int index = y * (xLod+1);

		for( int x=0; x < xLod; ++x )
		{
			// First triangle
			mesh->indexBuffer.push_back( index+1 );
			mesh->indexBuffer.push_back( index );
			mesh->indexBuffer.push_back( index+xLod+1 );
			// Second triangle
			mesh->indexBuffer.push_back( index+2+xLod );
			mesh->indexBuffer.push_back( index+1 );
			mesh->indexBuffer.push_back( index+1+xLod );

			++index;
		}
	}

	return mesh;
}

