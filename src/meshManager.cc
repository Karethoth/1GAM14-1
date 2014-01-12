#include "meshManager.hh"


MeshManager::~MeshManager()
{
}


bool MeshManager::Load( const std::string& filepath )
{
	return Load( filepath, filepath );
}


bool MeshManager::Load( const std::string& filepath,
                        const std::string& key )
{
	return false;
};

