#include "Camera.h"


Camera::Camera(vec3 posit, vec3 direct, vec3 up)
	:viewPosition(posit), viewDirection(direct), viewUp(up)
{
	this->update(0.0f);
}


Camera::~Camera(void)
{
}

void Camera::initialize()
{
	this->update(0.0f);

	VisualObject::initialize();

} // end initialize

void Camera::update(float elapsedTimeSeconds)
{
	viewMatrix = lookAt(viewPosition, viewPosition + viewDirection, viewUp );

	VisualObject::updateChildren(elapsedTimeSeconds);

} // end update
