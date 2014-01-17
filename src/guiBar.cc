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
	glVertex2f( realArea.x.value,                        realArea.y.value );
	glVertex2f( realArea.x.value + realArea.width.value, realArea.y.value );
	glVertex2f( realArea.x.value + realArea.width.value, realArea.y.value + realArea.height.value );
	glVertex2f( realArea.x.value,                        realArea.y.value + realArea.height.value );
	glEnd();
}

