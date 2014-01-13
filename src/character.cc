#include "character.hh"
#include "managers.hh"
#include "helpers.hh"
#include "../deb/glm/gtc/matrix_transform.hpp"
#include "../deb/glm/gtx/quaternion.hpp"


Character::Character( std::string characterName ) : Node( characterName )
{
	// Create the torso
	auto torso = std::make_shared<Entity>( "Torso" );
	torso->SetOrientation( glm::quat( glm::vec3(
		ToRadians( 90.f ),
		0.0,
		0.0
	) ) );
	torso->SetPosition( glm::vec3( -1.0f, 3.0, 0.0 ) );
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
	worldPosition    = position;
	worldOrientation = orientation;

	if( parent )
	{
		worldPosition    = parent->GetWorldPosition() +
		                   parent->GetWorldOrientation() * position;
		worldOrientation = parent->GetWorldOrientation() * orientation;
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

