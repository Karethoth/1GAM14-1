#include "character.hh"
#include "managers.hh"
#include "helpers.hh"
#include "../deb/glm/gtc/matrix_transform.hpp"
#include "../deb/glm/gtx/quaternion.hpp"

#include <algorithm>


extern bool ZCompare( const std::shared_ptr<Node>& a,
                      const std::shared_ptr<Node>& b );



Character::Character( std::string characterName ) : Node( characterName )
{
	// Create the head
	auto head = std::make_shared<Entity>( "Head" );
	head->SetOrientation( glm::quat( glm::vec3(
		ToRadians( 90.f ),
		0.0,
		0.0
	) ) );
	head->SetPosition( glm::vec3( -1.0f, 4.2, 0.2 ) );
	head->SetMeshName( "HeadMesh" );
	head->SetTextureName( "DefaultHead" );
	head->SetShaderName( "DefaultShader" );
	head->SetParent( static_cast<Node*>( this ) );
	bodyParts.push_back( head );

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
	bodyParts.push_back( torso );
}


Character::~Character()
{
	bodyParts.clear();
}



void Character::Render()
{
	glDisable( GL_CULL_FACE );
	for( auto& part : bodyParts )
	{
		part->Render();
	}
	glEnable( GL_CULL_FACE );
}


void Character::Update( double deltaTime )
{
	// Calculate new position
	glm::vec3 oldPosition = position;
	position += velocity * static_cast<float>( deltaTime );

	// Check if we have a collision
	bool collision = false;


	// Set orientation
	if( glm::length( velocity ) > 0.f )
	{
		glm::vec2 direction = glm::normalize( glm::vec2( velocity.x, velocity.z ) );

		float degrees = Cross( direction, glm::vec2( 0.0, 1.0 ) ) * 90.f;

		bool towards = true;
		if( Cross( direction, glm::vec2( 1.0, 0.0 ) ) > 0.0 )
		{
			towards = false;
			degrees *= -1.f;
		}

		SetOrientation( glm::quat( glm::vec3(
			0.f,
			ToRadians( degrees ),
			0.f
		) ) );
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
		part->UpdateWorldInfo();
	}

	std::sort( bodyParts.begin(), bodyParts.end(), ZCompare );
}



bool Character::CollidesWith( const AABB& otherCollisionBox )
{
	// For now, we won't be checking collision with children
	return AABBToAABBCollision( collisionBox+worldPosition, otherCollisionBox );
}



void Character::SetCollisionBox( AABB newCollisionBox )
{
	collisionBox = newCollisionBox;
}



void Character::SetVelocity( glm::vec3 newVelocity )
{
	velocity = newVelocity;
}


glm::vec3 Character::GetVelocity() const
{
	return velocity;
}

