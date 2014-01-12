#include "character.hh"
#include "managers.hh"
#include "helpers.hh"
#include "../deb/glm/gtc/matrix_transform.hpp"
#include "../deb/glm/gtx/quaternion.hpp"


Character::Character( std::string characterName ) : Node( characterName )
{
	// Create the torso
	auto torso = std::make_shared<Entity>( "Torso" );
	torso->SetRotation( glm::quat( glm::vec3( ToRadians( 90.f ), ToRadians( 180.f ), 0.0 ) ) );
	torso->SetPosition( glm::vec3( 1.0f, 3.0, 0.0 ) );
	torso->SetMeshName( "MediumTorsoMesh" );
	torso->SetTextureName( "DefaultTorso" );
	torso->SetShaderName( "DefaultShader" );
	torso->SetParent( static_cast<Node*>( this ) );
	bodyParts["torso"] = torso;
}


Character::~Character()
{
	bodyParts.clear();
}



void Character::Render()
{
	for( auto& part : bodyParts )
	{
		part.second->Render();
	}
}



void Character::UpdateWorldInfo()
{
	worldRotation = rotation;
	worldPosition = position;

	if( parent )
	{
		worldRotation = parent->GetWorldRotation() * rotation;

		worldPosition = parent->GetWorldPosition() +
		                parent->GetWorldRotation() * position;
	}

	// Update children
	for( auto& child : children )
	{
		child->UpdateWorldInfo();
	}

	// Update body parts
	for( auto& part : bodyParts )
	{
		part.second->UpdateWorldInfo();
	}
}

