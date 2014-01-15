#include "guiElement.hh"


GUIElement::GUIElement()
{
}


GUIElement::~GUIElement()
{
}



void GUIElement::SetSize( glm::vec2 newSize )
{
	size = newSize;
}


void GUIElement::SetPosition( glm::vec2 newPosition )
{
	position = newPosition;
}


void GUIElement::SetArea( const Box2D& newArea )
{
	position = newArea.point;
	size     = newArea.size;
}



void GUIElement::AddChild( const std::shared_ptr<GUIElement>& child )
{
	children.push_back( child );
}


void GUIElement::AddCallback( GUICallback callback )
{
	callbacks.push_back( callback );
}


void GUIElement::RemoveCallback( GUICallback callback )
{
	std::vector<GUICallback>::iterator it;
	for( it = callbacks.begin(); it != callbacks.end(); )
	{
		if( *it != callback )
		{
			++it;
			continue;
		}
		it = callbacks.erase( it );
	}
}



bool GUIElement::PointInArea( const glm::vec2& point )
{
	if( point.x >= position.x          &&
		point.x <  position.x + size.x &&
		point.y >= position.y          &&
		point.y <  position.y + size.y )
	{
		return true;
	}
	return false;
}



void GUIElement::Render()
{
}

