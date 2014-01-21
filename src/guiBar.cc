#include "guiBar.hh"
#include "managers.hh"
#include "../deb/glm/gtc/matrix_transform.hpp"

#include <iostream>


GUIBar::GUIBar()
{
}


GUIBar::~GUIBar()
{
}



void GUIBar::Render()
{
	auto texture = textureManager.Get( "b" );
	if( !texture )
	{
		texture = textureManager.Get( "DefaultTexture" );
	}

	// For now we'll do this in old fashion

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, texture->textureId );
	glUniform1i( shaderManager.Get( "DefaultGUIShader" )->GetUniform( "textureSampler" ), 0 );

	float textureScale = 0.2f;
	float ratio = realArea.width.value / realArea.height.value;
	float textureHeight = textureScale;
	float textureWidth  = textureScale * ratio;

	glBegin( GL_QUADS );
		glTexCoord2f( 0.f, 0.f );
		glVertex2f( realArea.x.value,                        realArea.y.value );
		glTexCoord2f( textureWidth, 0.f );
		glVertex2f( realArea.x.value + realArea.width.value, realArea.y.value );
		glTexCoord2f( textureWidth, textureHeight );
		glVertex2f( realArea.x.value + realArea.width.value, realArea.y.value + realArea.height.value );
		glTexCoord2f( 0.f, textureHeight );
		glVertex2f( realArea.x.value,                        realArea.y.value + realArea.height.value );
	glEnd();
}

