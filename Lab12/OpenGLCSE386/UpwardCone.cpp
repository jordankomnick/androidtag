#include "UpwardCone.h"


UpwardCone::UpwardCone( OpenGLApplicationBase * OpenGLApp, float height, float base, vec4 color, int numMajor, int numMinor)
	: Cone( OpenGLApp, height, base, color, numMajor, numMinor)
{
	mat4 fixedtrans = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f) );
	fixedTransformation = rotate( fixedtrans, -90.0f, vec3(1.0f, 0.0f, 0.0f) );
}


