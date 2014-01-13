#include "node.hh"
#include "../deb/glm/gtc/matrix_transform.hpp"
#include "../deb/glm/gtx/quaternion.hpp"

#include <iostream>
#include <algorithm>
#include <string>

using namespace glm;


static bool ZCompare( const std::shared_ptr<Node>& a,
                      const std::shared_ptr<Node>& b )
{
	return (a->GetWorldPosition().z < b->GetWorldPosition().z);
}



Node::Node( std::string name ) : name( name )
{
	parent = nullptr;

	SetScale( vec3( 1.f ) );
	SetPosition( vec3( 0.f ) );
	SetOrientation( quat( glm::vec3( 0.0, 0.0, 0.0 ) ) );
	UpdateWorldInfo();
}


Node::Node( const Node& other )
{
	name = other.name;
	position = other.position;
	orientation = other.orientation;
	worldPosition = other.worldPosition;
	worldOrientation = other.worldOrientation;
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
	std::swap( first.orientation, second.orientation );
	std::swap( first.worldPosition, second.worldPosition );
	std::swap( first.worldOrientation, second.worldOrientation );
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


void Node::SetOrientation( quat newOrientation )
{
	orientation = newOrientation;
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


quat Node::GetOrientation() const
{
	return orientation;
}


quat Node::GetWorldOrientation() const
{
	return worldOrientation;
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

	// Sort children
	std::sort( children.begin(), children.end(), ZCompare );
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

