#ifndef ___SPHERE_H___
#define ___SPHERE_H___

#include "visualobject.h"
#include "LoadShaders.h"

#include "Lighting.h"


class Sphere : public VisualObject
{
	public:
		Sphere( OpenGLApplicationBase * OpenGLApp, float Radius = 1.0f, int slices = 16, int stacks = 16, vec4 color = vec4( 1.0f, 0.0f, 0.0f, 1.0f) );
		virtual ~Sphere(void);

		virtual void initialize();

		virtual void draw();

	protected:
	
		void buildShaderProgram();

		void initializeSphereBody();
		void initializeSphereBottom();
		void initializeSphereTop();
		
		float radius;
		int stacks;
		int slices;

		GLuint vertexArrayObjectForBottom, vertexArrayObjectForTop; 

		// this variable will later be used to hold the number of indices
		int bodyIndicesCount, bottomIndicesCount, topIndicesCount;

		GLuint modelLocation;
		GLuint solidColorLocation;

		// For texture mapping
		GLuint textureObject;
		
		vec4 objectColor;
		Material material;

	// Step in z and radius as stacks are drawn.
	double z0, z1;
	double r0, r1;

	float stackStep;
	float sliceStep;
};

#endif /* ___SPHERE_H___ */

