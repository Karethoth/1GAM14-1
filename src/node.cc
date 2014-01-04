#include "node.hh"
#include <iostream>
#include <string>

using namespace glm;


Node::Node( std::string name ) : name( name )
{
	parent = nullptr;

	SetScale( vec3( 1.f ) );
	SetLocation( vec3( 0.f ) );
	SetRotation( quat( 0, 0, 0, 0 ) );

	//UpdateWorldInfo();
}


Node::Node( const Node& other )
{
	name = other.name;
	location = other.location;
	rotation = other.rotation;
	worldLocation = other.worldLocation;
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
	std::swap( first.location, second.location );
	std::swap( first.rotation, second.rotation );
	std::swap( first.worldLocation, second.worldLocation );
	std::swap( first.worldRotation, second.worldRotation );
	std::swap( first.parent, second.parent );
	std::swap( first.children, second.children );
}



void Node::SetScale( vec3 newScale )
{
	scale = newScale;
}



void Node::SetLocation( vec3 newLocation )
{
	location = newLocation;
}


void Node::SetRotation( quat newRotation )
{
	rotation = newRotation;
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



vec3 Node::GetLocation() const
{
	return location;
}



vec3 Node::GetWorldLocation() const
{
	return worldLocation;
}


quat Node::GetRotation() const
{
	return rotation;
}


quat Node::GetWorldRotation() const
{
	return worldRotation;
}



// Used to update worldLocation and worldPosition
void Node::UpdateWorldInfo()
{
	// Update this node
	if( parent )
	{
		worldLocation = parent->GetWorldLocation() +
		                parent->GetWorldRotation() * location;
		worldRotation = parent->GetRotation() + rotation;
	}
	else
	{
		worldLocation = location;
		worldRotation = rotation;
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

