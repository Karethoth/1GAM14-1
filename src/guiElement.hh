#pragma once
#ifndef _GUIELEMENT_HH_
#define _GUIELEMENT_HH_


#include "../deb/glm/glm.hpp"
#include <memory>
#include <vector>


typedef struct sBox2D
{
	glm::vec2 point;
	glm::vec2 size;
} Box2D;



enum GUIEventType
{
	UNDEFINED=0,
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
	unsigned int value;
	glm::vec2    point;
} GUIEvent;


typedef void (*GUICallback)(const GUIEvent&);



class GUIElement
{
 public:
	GUIElement();
	virtual ~GUIElement();

	virtual void SetSize( glm::vec2 newSize );
	virtual void SetPosition( glm::vec2 newPosition );
	virtual void SetArea( const Box2D& newArea );

	virtual void AddChild( const std::shared_ptr<GUIElement>& child );
	virtual void AddCallback( GUICallback callback );
	virtual void RemoveCallback( GUICallback callback );

	virtual bool PointInArea( const glm::vec2& point );

	virtual void Render();


 protected:
	std::vector<std::shared_ptr<GUIElement>> children;
	std::vector<GUICallback> callbacks;


 private:
	glm::vec2 position;
	glm::vec2 size;
};

#endif

