#pragma once
#include "controller.h"
class OrbitController :
	public Controller
{
public:

	OrbitController( float r = 10.0f, vec3 pos = vec3(0.0f, 0.0f, 0.0f) );

	// Update method that is call by the Visual Object.
	virtual void update(float elapsedTimeSeconds);

	protected:

	vec3 position;

	float radius;
};

