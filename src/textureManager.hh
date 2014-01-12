#pragma once
#ifndef _TEXTUREMANAGER_HH_
#define _TEXTUREMANAGER_HH_

#include "templateManager.hh"
#include "texture.hh"


class TextureManager : public TemplateManager<Texture>
{
 public:
	virtual ~TextureManager();

	virtual bool Load( const std::string& filepath );
	virtual bool Load( const std::string& filepath, const std::string& key );
};


#endif

