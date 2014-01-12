#include "node.hh"
#include "../deb/glm/gtc/matrix_transform.hpp"
#include "../deb/glm/gtx/quaternion.hpp"

#include <iostream>
#include <string>

using namespace glm;


Node::Node( std::string name ) : name( name )
{
	parent = nullptr;

	SetScale( vec3( 1.f ) );
	SetPosition( vec3( 0.f ) );
	SetRotation( quat( glm::vec3( 0.0, 0.0, 0.0 ) ) );
	UpdateWorldInfo();
}


Node::Node( const Node& other )
{
	name = other.name;
	position = other.position;
	rotation = other.rotation;
	worldPosition = other.worldPosition;
	worldRotation = other.worldRotation;
	parent = other.parent;
	children = other.children;
}


Node::Node( Node&& other )
{
	Swap( *this, other );
}


Node& Node::operator = ( Node other )
{
	Swap( *this, other );
	return *this;
}


Node::~Node()
{
	children.clear();
}



void Node::Swap( Node& first, Node& second )
{
	std::swap( first.name, second.name );
	std::swap( first.position, second.position );
	std::swap( first.rotation, second.rotation );
	std::swap( first.worldPosition, second.worldPosition );
	std::swap( first.worldRotation, second.worldRotation );
	std::swap( first.parent, second.parent );
	std::swap( first.children, second.children );
}



void Node::SetScale( vec3 newScale )
{
	scale = newScale;
}



void Node::SetPosition( vec3 newPosition )
{
	position = newPosition;
	//UpdateWorldInfo();
}


void Node::SetRotation( quat newRotation )
{
	rotation = newRotation;
	//UpdateWorldInfo();
}



void Node::SetName( std::string newName )
{
	name = newName;
}


std::string Node::GetName()
{
	return name;
}



vec3 Node::GetScale() const
{
	return scale;
}



vec3 Node::GetPosition() const
{
	return position;
}



vec3 Node::GetWorldPosition() const
{
	return worldPosition;
}


quat Node::GetRotation() const
{
	return rotation;
}


quat Node::GetWorldRotation() const
{
	return worldRotation;
}



// Used to recursively go trough subentities and render them.
void Node::Render()
{
	// Render children
	for( auto& child : children )
	{
		child->Render();
	}
}



// Used to update worldLocation and worldPosition
void Node::UpdateWorldInfo()
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
}



void Node::AddChild( const std::shared_ptr<Node>& child )
{
	children.push_back( child );
	child->SetParent( this );
}



void Node::EraseChild( const std::shared_ptr<Node>& child )
{
	std::vector<std::shared_ptr<Node>>::iterator it;

	for( it = children.begin(); it != children.end(); ++it )
	{
		if( it->get() == child.get() )
		{
			children.erase( it );
			child->SetParent( nullptr );
			return;
		}
	}
}



void Node::SetParent( Node* parent )
{
	this->parent = parent;
}

