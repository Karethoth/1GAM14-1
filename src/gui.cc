#include "gui.hh"
#include <iostream>


GUI::GUI()
{
	shaderName = "DefaultGUIShader";
}


GUI::~GUI()
{
}



void GUI::Render()
{
	auto shader = shaderManager.Get( shaderName );
	if( !shader )
	{
		std::cerr << "GUI couldn't find shader program '" << shaderName << "'!\n";
		return;
	}

	glDisable( GL_DEPTH );

	glUseProgram( shader->Get() );

	for( auto& child : children )
	{
		child->Render();
	}

	glEnable( GL_DEPTH );
}

