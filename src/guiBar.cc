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
		glVertex2f( position.x,          position.y );
		glVertex2f( position.x + size.x, position.y );
		glVertex2f( position.x + size.x, position.y + size.y );
		glVertex2f( position.x,          position.y + size.y );
	glEnd();
}

