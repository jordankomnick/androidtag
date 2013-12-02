#pragma once
#include "controller.h"
class SpinnerController :	public Controller
{
public:

	// Constructor sets the position of the object,
	// the axis about which it will rotate, and the rate and direction
	// of rotation.
	SpinnerController(vec3 pos = vec3(0.0f, 0.0f, 0.0f), vec3 ax = vec3(1.0, 0.0f, 0.0f), GLfloat r = 10.0f);

	virtual void update(float elapsedTimeSeconds);

protected:
	
	// Position at which the object is spinning in place
	vec3 position; 

	// axis of rotation
	vec3 axis; 

	// rate and direction of rotation
	GLfloat rate; 

	// Current rotation angle
	float rotation;
};

