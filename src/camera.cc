#include "camera.hh"
#include "../deb/glm/gtc/matrix_transform.hpp"
#include "../deb/glm/gtx/quaternion.hpp"

using namespace glm;


Camera::Camera( std::string name ) : Node( name )
{
	SetFOV( 60 );
	SetRatio( 4.f/3.f );
	SetZLimits( 0.1f, 100.f );
	SetTarget( vec3( 0 ) );
	SetUpVector( vec3( 0, 1, 0 ) );
}


Camera::~Camera()
{
}



std::shared_ptr<mat4> Camera::GetViewMatrix()
{
	auto view = std::make_shared<mat4>(
		glm::lookAt(
			GetWorldPosition(),
			target,
			GetWorldRotation() * upVector
		)
	);
	return view;
}



std::shared_ptr<mat4> Camera::GetProjectionMatrix()
{
	// This matrix probably should be saved and updated just when necessary.
	auto projection = std::make_shared<mat4>(
	    glm::perspective( fieldOfView, aspectRatio, zNear, zFar )
	);

	return projection;
}



void Camera::SetFOV( float fov )
{
	fieldOfView = fov;
}


void Camera::SetRatio( float ratio )
{
	aspectRatio = ratio;
}


void Camera::SetZLimits( float near, float far )
{
	zNear = near;
	zFar  = far;
}


void Camera::SetTarget( glm::vec3 newTarget )
{
	target = newTarget;
}


void Camera::SetUpVector( glm::vec3 up )
{
	upVector = up;
}

