#include "entity.hh"
#include <memory>

Entity::Entity( std::string entityName ) : Node( entityName )
{
	meshName = "defaultMesh";
}


Entity::~Entity()
{
}



void Entity::SetMeshName( std::string newMeshName )
{
	meshName = newMeshName;
}


std::string Entity::GetMeshName()
{
	return meshName;
}



void Entity::Render()
{
	// TODO: Rendering goes here.


	for( auto& child : children )
	{
		auto entityChild = std::dynamic_pointer_cast<Entity>( child );
		if( !entityChild )
		{
			entityChild->Render();
		}
	}
}

