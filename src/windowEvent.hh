#pragma once
#ifndef _WINDOWEVENT_HH_
#define _WINDOWEVENT_HH_

#include "../deb/glm/glm.hpp"


enum WindowEventType
{
	WINDOW_MOVE,
	WINDOW_RESIZE,
	WINDOW_FRAMEBUFFER_RESIZE,
};


struct WindowEvent
{
	WindowEventType type;
	glm::vec2 point;
};


#endif

