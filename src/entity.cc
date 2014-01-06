#include "entity.hh"
#include "managers.hh"
#include "../deb/glm/gtc/matrix_transform.hpp"
#include "../deb/glm/gtx/quaternion.hpp"

#include <iostream>
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


static glm::mat4 modelMatrix;


void Entity::Render()
{
	auto mesh = meshManager.Get( meshName );
	auto shader = shaderManager.Get( shaderName );

	if( !parent )
	{
		modelMatrix = glm::mat4( 1.0 );
	}

	auto savedMatrix = modelMatrix;

	modelMatrix =  modelMatrix * glm::toMat4( GetRotation() );
	modelMatrix = glm::translate( modelMatrix, GetLocation() );

	if( mesh && shader )
	{
		// Use the vertex array object of the mesh
		glBindVertexArray( mesh->vao );
		const GLint modelUniform  = shader->GetUniform( "M" );

		glUniformMatrix4fv( modelUniform, 1, GL_FALSE, &modelMatrix[0][0] );

		// Draw the elements
		glDrawElements(
			GL_TRIANGLES,
			mesh->indexBuffer.size(),
			GL_UNSIGNED_INT,
			(void*)0
		);
	}
	else
	{
		if( !mesh )
		{
			std::cerr << "Error: Entity '" << GetName() << "' is trying to "
		                 "use mesh '" << meshName << "' which can't be found!\n";
		}
		if( !shader )
		{
			std::cerr << "Error: Entity '" << GetName() << "' is trying to "
		                 "use shader '" << shaderName << "' which can't be found!\n";
		}
	}


	for( auto& child : children )
	{
		auto entityChild = std::dynamic_pointer_cast<Entity>( child );
		if( entityChild )
		{
			entityChild->Render();
		}
	}

	modelMatrix = savedMatrix;
}

