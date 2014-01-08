#pragma once
#ifndef _HELPERS_HH_
#define _HELPERS_HH_

#include "../deb/glm/glm.hpp"
#include <memory>
#include <vector>


float  ToRadians( float degrees );
double ToRadians( double degrees );


float Cross( const glm::vec2& a, const glm::vec2& b );

std::shared_ptr<glm::vec2> PointInBetween( const glm::vec2& a,
                                           const glm::vec2& b,
                                           double ratio );

std::shared_ptr<glm::vec2> Intersection( const glm::vec2& a,
                                         const glm::vec2& bOffset,
                                         const glm::vec2& c,
                                         const glm::vec2& dOffset );


float Noise2D( float x, float y );

std::shared_ptr<std::vector<float>> NoiseArray( int width=10, int height=10,
                                                int startX=0, int startY=0 );

#endif

