#pragma once
#ifndef _GUI_HH_
#define _GUI_HH_

#include "guiElement.hh"
#include "managers.hh"
#include <vector>


class GUI : public GUIElement
{
 public:
	GUI();
	virtual ~GUI();

	virtual void Render();

	void SetShaderName( std::string newShaderName );


 private:
	std::string shaderName;
};

#endif

