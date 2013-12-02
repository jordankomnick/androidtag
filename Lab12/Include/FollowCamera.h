#ifndef ___FOLLOW_CAMERA_H___
#define ___FOLLOW_CAMERA_H___

#include "ListenerCamera.h"
#include "WaypointController2.h"

class FollowCamera : public ListenerCamera
{
public:
	FollowCamera(vec3 posit = vec3(0.0f, 0.0f, 12.0f), vec3 direct = vec3(0.0f, 0.0f, -1.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f));
	~FollowCamera(void);

	virtual void update(float elapsedTimeSeconds);

	WaypointController2 * cont;
};

#endif // ___FOLLOW_CAMERA_H___