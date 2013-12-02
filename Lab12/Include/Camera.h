#ifndef ___CAMERA_H___
#define ___CAMERA_H___

#include "visualobject.h"
class Camera :
	public VisualObject
{
public:
	Camera(vec3 posit = vec3(0.0f, 0.0f, 12.0f), vec3 direct = vec3(0.0f, 0.0f, -1.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f));
	~Camera(void);

	virtual void initialize();

	virtual void update(float elapsedTimeSeconds);

	mat4 getViewMatrix() { return viewMatrix; };

	vec3 getViewPosition() { return viewPosition; };
	void setViewPosition(vec3 posit) { viewPosition = posit; };

	vec3 getViewDirection() { return viewDirection; };
	void setViewDirection(vec3 direct) { viewDirection = direct; };

	vec3 getViewUp() { return viewUp; };
	void setViewUp(vec3 up) { viewUp = up; };

protected:

	mat4 viewMatrix;

	vec3 viewPosition;
	vec3 viewDirection;
	vec3 viewUp;

};

#endif // ___CAMERA_H___

