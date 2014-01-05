#pragma once
#ifndef _CAMERA_HH_
#define _CAMERA_HH_

#include "node.hh"


class Camera : public Node
{
 public:
	Camera( std::string name="UnnamedCamera" );
	virtual ~Camera();

	std::shared_ptr<glm::mat4> GetViewMatrix();
	std::shared_ptr<glm::mat4> GetProjectionMatrix();

	void SetFOV( float fov );
	void SetRatio( float ratio );
	void SetZLimits( float near, float far );
	void SetTarget( glm::vec3 newTarget );
	void SetUpVector( glm::vec3 up );


 private:
	 float fieldOfView;
	 float aspectRatio;
	 float zNear, zFar;
	 glm::vec3 target;
	 glm::vec3 upVector;
};


#endif

