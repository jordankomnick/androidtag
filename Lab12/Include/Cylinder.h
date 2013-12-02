#ifndef ___CYLINDER_H___
#define ___CYLINDER_H___

#include "visualobject.h"
#include "LoadShaders.h"
#include "Lighting.h"


class Cylinder : public VisualObject
{
	public:
		Cylinder( OpenGLApplicationBase * OpenGLApp, float height = 2.0f, float radius = 0.5f, vec4 color = vec4( 0.0f, 0.0f, 1.0f, 1.0f), int numMajor = 4, int numMinor = 16);

		virtual ~Cylinder(void);
			
		virtual void initialize();

		virtual void draw();

	protected:
	
		void buildShaderProgram();

		void initializeCylinderBody();
		void initializeCylinderBottom();
		void initializeCylinderTop();

		int stacks, slices;
		float height, radius;

		GLuint vertexArrayObjectForBottom, vertexArrayObjectForTop; 

		// this variable will later be used to hold the number of indices
		int bodyIndicesCount, bottomIndicesCount, topIndicesCount;

		// For texture mapping
		GLuint textureObject;

		GLuint modelLocation;
		GLuint solidColorLocation;

		vec4 objectColor;
		Material material;
};

#endif /* ___CYLINDER_H___ */

