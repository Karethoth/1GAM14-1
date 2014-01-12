#pragma once
#ifndef _NODE_HH_
#define _NODE_HH_

#include <string>
#include <memory>
#include <vector>

#include "../deb/glm/gtc/quaternion.hpp"
#include "../deb/glm/glm.hpp"


class Node
{
 public:
	Node( std::string name="NoName" );
	virtual ~Node();

	Node( const Node& other );
	Node(  Node&& other );
	Node& operator = ( Node other );
	void Swap( Node& first, Node& second );

	void SetName( std::string );
	std::string GetName();

	void SetScale( glm::vec3 newScale );
	void SetPosition( glm::vec3 newPosition );
	void SetRotation( glm::quat newRotation );

	glm::vec3 GetScale() const;
	glm::vec3 GetPosition() const;
	glm::vec3 GetWorldPosition() const;
	glm::quat GetRotation() const;
	glm::quat GetWorldRotation() const;

	virtual void Render();
	virtual void UpdateWorldInfo();

	void AddChild( const std::shared_ptr<Node>& child );
	void EraseChild( const std::shared_ptr<Node>& child );

	std::vector<std::shared_ptr<Node>> GetChildren();

	void SetParent( Node* parent );


 protected:
	Node* parent;
	std::vector<std::shared_ptr<Node>> children;

	std::string name;
	glm::vec3 scale;
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 worldPosition;
	glm::quat worldRotation;

};

#endif

