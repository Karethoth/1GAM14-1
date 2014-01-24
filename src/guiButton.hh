#pragma once
#ifndef _GUIBUTTON_HH_
#define _GUIBUTTON_HH_

#include "guiElement.hh"
#include <vector>


class GUIButton : public GUIElement
{
 public:
	GUIButton();
	virtual ~GUIButton();

	virtual void Render();
	virtual void HandleInputEvent( const InputEvent& event );
};

#endif

