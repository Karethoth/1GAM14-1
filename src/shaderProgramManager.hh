#pragma once
#ifndef _SHADERPROGRAMMANAGER_HH_
#define _SHADERPROGRAMMANAGER_HH_

#include "templateManager.hh"
#include "shaderProgram.hh"


class ShaderProgramManager : public TemplateManager<ShaderProgram>
{
 public:
	virtual ~ShaderProgramManager();

	virtual bool Load( std::string &programFile );
};


#endif

