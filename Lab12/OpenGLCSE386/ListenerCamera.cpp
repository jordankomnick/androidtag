#include "ListenerCamera.h"


ListenerCamera::ListenerCamera(vec3 posit, vec3 direct, vec3 up)
	:Camera(posit,direct,viewUp)
{
	this->update(0.0f);
}


void ListenerCamera::update(float elapsedTimeSeconds)
{
	Camera::update(elapsedTimeSeconds);

	static vec3 lastViewPosition = viewPosition;

	alListener3f(AL_POSITION, viewPosition.x, viewPosition.y, viewPosition.z );

	float orientation[] = { viewDirection.x, viewDirection.y, viewDirection.z,
							viewUp.x, viewUp.y, viewUp.z };

	alListenerfv(AL_ORIENTATION, orientation );

	if ( elapsedTimeSeconds > 0) {
		vec3 velocity  = (viewPosition - lastViewPosition)/elapsedTimeSeconds;
		alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z );
	
	}

	lastViewPosition = viewPosition;

	VisualObject::updateChildren(elapsedTimeSeconds);

} // end update
