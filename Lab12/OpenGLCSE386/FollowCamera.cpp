#include "FollowCamera.h"


FollowCamera::FollowCamera(vec3 posit, vec3 direct, vec3 up)
	: ListenerCamera(posit, direct, up), cont(NULL)
{
}


FollowCamera::~FollowCamera(void)
{
}

void FollowCamera::update(float elapsedTimeSeconds)
{
	if (cont != NULL) {

		viewDirection = vec3(0.0, -1.0f, 0.0f);

		viewPosition = cont->getPosition() + vec3(0.0f, 6.0f, 0.0f);

		viewUp = vec3(0, 0, 1);//normalize(cont->getVelocity());
		
	}

	ListenerCamera::update(elapsedTimeSeconds);

} // end update