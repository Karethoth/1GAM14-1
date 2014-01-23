#pragma once
#ifndef _GUIELEMENT_HH_
#define _GUIELEMENT_HH_


#include "../deb/glm/glm.hpp"
#include "input.hh"
#include <memory>
#include <vector>


enum GUIMeasureType
{
	DIRECT,
	PERCENTAGE
};


struct GUIMeasure
{
	float          value;
	GUIMeasureType type;
	GUIMeasure( float value=0.f, GUIMeasureType type=DIRECT );
};



struct Box2D
{
	GUIMeasure x;
	GUIMeasure y;
	GUIMeasure width;
	GUIMeasure height;
};



// Forward declare the GUIElement.
class GUIElement;


enum GUIEventType
{
	UNDEFINED_GUI_EVENT = 0,
	ELEMENT_SELECTED
};


struct GUIEvent
{
	std::shared_ptr<GUIElement> element;
	GUIEventType type;
};


typedef void (*GUICallback)(const GUIEvent&);



class GUIElement
{
 public:
	GUIElement();
	virtual ~GUIElement();

	virtual Box2D GetRealArea();

	virtual void SetSize( GUIMeasure width, GUIMeasure height );
	virtual void SetPosition( GUIMeasure x, GUIMeasure y );
	virtual void SetArea( const Box2D& newArea );

	virtual void SetAlpha( float newAlpha );

	virtual void SetParent( GUIElement* newParent );
	virtual void AddChild( const std::shared_ptr<GUIElement>& child );
	virtual void AddCallback( GUICallback callback );
	virtual void RemoveCallback( GUICallback callback );

	virtual void HandleInputEvent( const InputEvent& event );
	virtual bool PointInArea( const glm::vec2& point );

	virtual void UpdateRealArea();
	virtual void Render();


 protected:
	Box2D area;
	Box2D realArea;

	float alpha;

	GUIElement *parent;
	std::vector<std::shared_ptr<GUIElement>> children;
	std::vector<GUICallback> callbacks;
};

#endif

