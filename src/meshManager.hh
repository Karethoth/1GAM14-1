#pragma once
#ifndef _MESHMANAGER_HH_
#define _MESHMANAGER_HH_

#include "templateManager.hh"
#include "mesh.hh"


class MeshManager : public TemplateManager<Mesh>
{
 public:
	virtual ~MeshManager();

	// Doesn't do a thing.
	virtual bool Load( const std::string& filepath );
	virtual bool Load( const std::string& filepath, const std::string& key );
};


#endif

