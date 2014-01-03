#include "node.hh"
#include <iostream>
#include <string>

using namespace glm;

Node::Node()
{
	Node( "NoName" );
}

Node::Node( const std::string &name ) : name( name )
{
	SetLocation( vec3( 0, 0, 0 ) );
	SetRotation( quat( 0, 0, 0, 0 ) );

	//UpdateWorldInfo();
}


Node::~Node()
{
	children.clear();
}



void Node::SetLocation( const vec3 &loc )
{
	location = loc;
}


void Node::SetRotation( const quat &rot )
{
	rotation = rot;
}



std::string Node::GetName()
{
	return name;
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
	std::shared_ptr<Node> pParent = parent.lock();

	// Update this node
	if( pParent )
	{
		worldLocation = pParent->GetWorldLocation() +
		                pParent->GetWorldRotation() * location;
		worldRotation = pParent->GetRotation() + rotation;
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



void Node::AddChild( const std::shared_ptr<Node> &child )
{
	children.push_back( child );
	child->SetParent( std::make_shared<Node>( *this ) );
}



void Node::EraseChild( const std::shared_ptr<Node> &child )
{
	std::vector<std::shared_ptr<Node>>::iterator it;

	for( it = children.begin(); it != children.end(); ++it )
	{
		if( it->get() == child.get() )
		{
			children.erase( it );
			child->SetParent( std::shared_ptr<Node>( nullptr ) );
			return;
		}
	}
}



void Node::SetParent( const std::shared_ptr<Node> &parent )
{
	this->parent = parent;
}



void Node::Delete()
{
	std::shared_ptr<Node> pParent = parent.lock();
	if( pParent )
	{

	}
}

