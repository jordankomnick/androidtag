#pragma once
#include "cone.h"
class UpwardCone : public Cone
{
public:
	
	UpwardCone( OpenGLApplicationBase * OpenGLApp, float height = 2.0f, float base = 0.5f, vec4 color = vec4( 1.0f, 0.5f, 0.5f, 1.0f), int numMajor = 4, int numMinor = 16);

};

