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

