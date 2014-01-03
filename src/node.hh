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
	Node();
	Node( const std::string &name );
	~Node();

	Node( const Node &other );
	Node(  Node &&other );
	Node& operator = ( Node other );
	void Swap( Node &first, Node &second );

	void SetName( std::string );
	std::string GetName();

	void SetLocation( const glm::vec3 &loc );
	void SetRotation( const glm::quat &rot );


	glm::vec3 GetLocation() const;
	glm::vec3 GetWorldLocation() const;
	glm::quat GetRotation() const;
	glm::quat GetWorldRotation() const;

	void UpdateWorldInfo();

	void AddChild( const std::shared_ptr<Node> &child );
	void EraseChild( const std::shared_ptr<Node> &child );

	void Delete();


 protected:
	void SetParent( Node *parent );


 private:
	std::string name;

	glm::vec3 location;
	glm::quat rotation;
	glm::vec3 worldLocation;
	glm::quat worldRotation;

	Node *parent;
	std::vector<std::shared_ptr<Node>> children;
};

#endif
