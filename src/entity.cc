#include "entity.hh"
#include <memory>

#include "meshManager.hh"

extern MeshManager meshManager;


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
	auto mesh = meshManager.Get( meshName );

	if( mesh )
	{
		// Draw the elements
		glDrawElements(
			GL_TRIANGLES,
			mesh->indexBuffer.size(),
			GL_UNSIGNED_INT,
			(void*)0
		);
	}

	for( auto& child : children )
	{
		auto entityChild = std::dynamic_pointer_cast<Entity>( child );
		if( !entityChild )
		{
			entityChild->Render();
		}
	}
}

