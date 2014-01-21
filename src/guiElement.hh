#pragma once
#ifndef _GUIELEMENT_HH_
#define _GUIELEMENT_HH_


#include "../deb/glm/glm.hpp"
#include <memory>
#include <vector>


enum GUIMeasureType
{
	DIRECT,
	PERCENTAGE
};


typedef struct sGUIMeasure
{
	float          value;
	GUIMeasureType type;
	sGUIMeasure( float value=0.f, GUIMeasureType type=DIRECT );
} GUIMeasure;



typedef struct sBox2D
{
	GUIMeasure x;
	GUIMeasure y;
	GUIMeasure width;
	GUIMeasure height;
} Box2D;



enum GUIEventType
{
	UNDEFINED_GUI_EVENT = 0,
	MOUSE_DOWN,
	MOUSE_UP,
	MOUSE_MOVE,
	MOUSE_SCROLL,
	KEY_DOWN,
	KEY_UP
};


// Forward declare the GUIElement.
class GUIElement;


typedef struct sGUIEvent
{
	std::shared_ptr<GUIElement> element;
	GUIEventType type;
	int          button;
	int          scancode;
	int          value;
	int          mods;
	glm::vec2    point;
	glm::vec2    scroll;
} GUIEvent;


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

	virtual void HandleEvent( const GUIEvent& event );
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

