#ifndef ___LISTENER_CAMERA_H___
#define ___LISTENER_CAMERA_H___

#include "camera.h"
#include "Framework.h" // For sound stuff


class ListenerCamera : public Camera
{
public:
	ListenerCamera(vec3 posit = vec3(0.0f, 0.0f, 12.0f), vec3 direct = vec3(0.0f, 0.0f, -1.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f));

	virtual void update(float elapsedTimeSeconds);

};

#endif // ___LISTENER_CAMERA_H___

