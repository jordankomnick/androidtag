#include "textureCoordinates.h"

#define M_PI 3.1415926535897932384626433832795f

vec2 getCylindericalTextCoords( float theta, vec3 vertex, float height )
{
	float s = theta/(2.0f *M_PI);

	float t = vertex.z /height;

	return vec2(s, t);
}

// Get the Spherical texture mapping coordinates
vec2 getSphericalTextCoords(float theta, vec3 vertex)
{
	vec3 d  = normalize(vertex);

	float s = theta/(2.0f * M_PI);

	float phi = acos(-d.z);
	float t = phi/M_PI;

	return vec2(s,t);
}

vec2 getPlanarTextCoords( vec3 vertex, float width, float height )
{
	float s = vertex.x + width/2;
	float t = vertex.z/height;

	return vec2(s, t);
}