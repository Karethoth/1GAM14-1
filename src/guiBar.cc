#include "guiBar.hh"
#include "managers.hh"
#include "../deb/glm/gtc/matrix_transform.hpp"

#include <iostream>


GUIBar::GUIBar()
{
}


GUIBar::~GUIBar()
{
}



void GUIBar::Render()
{
	// For now we'll do this in old fashion
	glBegin( GL_QUADS );
	glVertex2f( area.x.value,                    area.y.value );
	glVertex2f( area.x.value + area.width.value, area.y.value );
	glVertex2f( area.x.value + area.width.value, area.y.value + area.height.value );
	glVertex2f( area.x.value,                    area.y.value + area.height.value );
	glEnd();
}

