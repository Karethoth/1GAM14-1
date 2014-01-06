#include "entity.hh"
#include "managers.hh"
#include "../deb/glm/gtc/matrix_transform.hpp"
#include "../deb/glm/gtx/quaternion.hpp"

#include <memory>


Entity::Entity( std::string entityName ) : Node( entityName )
{
	meshName   = "defaultMesh";
	shaderName = "defaultShader";
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



void Entity::SetShaderName( std::string newShaderName )
{
	shaderName = newShaderName;
}


std::string Entity::GetShaderName()
{
	return shaderName;
}



void Entity::Render()
{
	// TODO: Rendering goes here.
	auto mesh = meshManager.Get( meshName );
	auto shader = shaderManager.Get( shaderName );

	if( mesh && shader )
	{
		const GLint modelUniform  = shader->GetUniform( "M" );

		glm::mat4 modelMat = glm::mat4( 1.0f );
		modelMat = modelMat * glm::toMat4(  GetWorldRotation() );
		modelMat = glm::translate(  modelMat, GetWorldLocation() );
		glUniformMatrix4fv( modelUniform, 1, GL_FALSE, &modelMat[0][0] );

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

