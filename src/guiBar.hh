#pragma once
#ifndef _GUIBAR_HH_
#define _GUIBAR_HH_

#include "guiElement.hh"
#include <vector>


class GUIBar : public GUIElement
{
 public:
	GUIBar();
	virtual ~GUIBar();

	virtual void Render();
	virtual void HandleEvent( const GUIEvent& event );
};

#endif

