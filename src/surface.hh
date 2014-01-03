#pragma once
#ifndef _SURFRACE_HH_
#define _SURFRACE_HH_

#include "mesh.hh"

/*
 * The texture cornes for Surfaces:
 * --------------------------------
 *    0------1
 *    |      |
 *    2------3
 */

struct Surface
{
	Surface();
	Surface( float width, float height );

	void SetTextureCorners( const glm::vec2 corners[4]  );

	Mesh GenerateMesh( int xSplits, int ySplits );

	glm::vec2 size;
	glm::vec2 textureCorners[4];
};

#endif
