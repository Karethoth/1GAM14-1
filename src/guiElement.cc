#include "guiElement.hh"


GUIMeasure::sGUIMeasure( float value, GUIMeasureType type )
{
	this->value = value;
	this->type  = type;
}

GUIElement::GUIElement()
{
}


GUIElement::~GUIElement()
{
}



void GUIElement::SetSize( GUIMeasure newWidth, GUIMeasure newHeight )
{
	area.width  = newWidth;
	area.height = newHeight;
}


void GUIElement::SetPosition( GUIMeasure x, GUIMeasure y )
{
	area.x = x;
	area.y = y;
}


void GUIElement::SetArea( const Box2D& newArea )
{
	area = newArea;
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
	if( point.x >= realArea.x.value                        &&
		point.x <  realArea.x.value + realArea.width.value &&
		point.y >= realArea.y.value                        &&
		point.y <  realArea.y.value + realArea.height.value )
	{
		return true;
	}
	return false;
}



void GUIElement::Render()
{
	for( auto& child : children )
	{
		child->Render();
	}
}

