#include "gui.hh"
#include "managers.hh"
#include "../deb/glm/gtc/matrix_transform.hpp"

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

	glUseProgram( shader->Get() );


	// For now, let's do it inefficiently
	GLint projUniform = shader->GetUniform( "P" );
	glm::mat4 projectionMat = glm::ortho<float>(
		0.f,
		area.width.value,
		area.height.value,
		0.f
	);

	glUniformMatrix4fv( projUniform, 1, GL_FALSE, &(projectionMat)[0][0] );
	GLint modelUniform = shader->GetUniform( "M" );
	glm::mat4 modelMat = glm::translate( glm::mat4( 1.f ), glm::vec3( 0.375, 0.375, 0.0 ) );
	glUniformMatrix4fv( modelUniform, 1, GL_FALSE, &(modelMat)[0][0] );

	glDepthMask( GL_FALSE );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );

	for( auto& child : children )
	{
		child->Render();
	}

	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glDepthMask( GL_TRUE );
}

