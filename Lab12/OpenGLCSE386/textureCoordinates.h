#ifndef ___TEXTURE_COORDINATES_H___
#define ___TEXTURE_COORDINATES_H___

#define GLM_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/swizzle.hpp>
#include <glm/gtc/constants.hpp>

using namespace glm;

//u -> theta
//v ->z
// x = r * cos(theta), y = r * sin(theta)
vec2 getCylindericalTextCoords( float theta, vec3 vertex, float height );

// Spherical Mapping
//(x,y,z) -> (latitude,longitude)-> (u,v)
vec2 getSphericalTextCoords(float theta, vec3 vertex);

//Planar mapping
// Use two of the coordinates normalized to the the [0,1] as the u and v coordinates
vec2 getPlanarTextCoords( vec3 vertex, float width, float height );


#endif // ___TEXTURE_COORDINATES_H___