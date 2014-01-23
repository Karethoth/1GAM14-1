#include "guiElement.hh"
#include <iostream>


GUIMeasure::sGUIMeasure( float value, GUIMeasureType type )
{
	this->value = value;
	this->type  = type;
}



GUIElement::GUIElement()
{
	parent = nullptr;
	alpha  = 1.f;
}


GUIElement::~GUIElement()
{
}



Box2D GUIElement::GetRealArea()
{
	return realArea;
}



void GUIElement::SetSize( GUIMeasure newWidth, GUIMeasure newHeight )
{
	area.width  = newWidth;
	area.height = newHeight;
	UpdateRealArea();
}


void GUIElement::SetPosition( GUIMeasure x, GUIMeasure y )
{
	area.x = x;
	area.y = y;
	UpdateRealArea();
}


void GUIElement::SetArea( const Box2D& newArea )
{
	area = newArea;
	UpdateRealArea();
}


void GUIElement::SetAlpha( float newAlpha )
{
	alpha = newAlpha;
}


void GUIElement::SetParent( GUIElement* newParent )
{
	parent = newParent;
	UpdateRealArea();
}


void GUIElement::AddChild( const std::shared_ptr<GUIElement>& child )
{
	child->SetParent( this );
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



void GUIElement::HandleInputEvent( const InputEvent& event )
{
	for( auto& child : children )
	{
		child->HandleInputEvent( event );
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



void GUIElement::UpdateRealArea()
{
	if( !parent )
	{
		realArea = area;
		if( area.x.type      == PERCENTAGE ||
			area.y.type      == PERCENTAGE ||
			area.width.type  == PERCENTAGE ||
			area.height.type == PERCENTAGE )
		{
			// Okay, having percentage without a parent
			// element is... weird. Show a warning.
			std::cerr << "Warning: Root GUIElement has percentual area!\n";
		}
	}
	else
	{
		auto parentArea = parent->GetRealArea();
		float newX      = parentArea.x.value;
		float newY      = parentArea.y.value;
		float newWidth  = 0.f;
		float newHeight = 0.f;

		// Calculate the position
		if( area.x.type == PERCENTAGE )
		{
			newX += parentArea.width.value / 100.f * area.x.value;
		}
		else
		{
			newX += area.x.value;
		}

		if( area.y.type == PERCENTAGE )
		{
			newY += parentArea.width.value / 100.f * area.y.value;
		}
		else
		{
			newY += area.y.value;
		}


		// Calculate the size
		if( area.width.type == PERCENTAGE )
		{
			newWidth = parentArea.width.value / 100.f * area.width.value;
		}
		else
		{
			newWidth += area.width.value;
		}

		if( area.height.type == PERCENTAGE )
		{
			newHeight = parentArea.height.value / 100.f * area.height.value;
		}
		else
		{
			newHeight += area.height.value;
		}

		realArea.x      = newX;
		realArea.y      = newY;
		realArea.width  = newWidth;
		realArea.height = newHeight;
	}

	// And as usual, update children too
	for( auto& child : children )
	{
		child->UpdateRealArea();
	}
}



void GUIElement::Render()
{
	for( auto& child : children )
	{
		child->Render();
	}
}

