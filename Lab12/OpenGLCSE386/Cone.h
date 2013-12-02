#ifndef ___CONE_H___
#define ___CONE_H___

#include "visualobject.h"
#include "LoadShaders.h"
#include "Lighting.h"

class Cone : public VisualObject
{
	public:
		Cone( OpenGLApplicationBase * OpenGLApp, 
			float height = 2.0f, float base = 0.5f, 
			vec4 color = vec4( 1.0f, 1.0f, 0.0f, 1.0f), 
			int numMajor = 4, int numMinor = 16);

		virtual ~Cone(void);
			
		virtual void initialize();

		virtual void draw();

	protected:
	
		void buildShaderProgram();

		void initializeConeBody();
		void initializeConeBottom();

		int stacks, slices;
		float height, base;

		GLuint vertexArrayObjectForBottom; 

		// this variable will later be used to hold the number of indices
		int bodyIndicesCount, bottomIndicesCount;

		// For texture mapping
		GLuint textureObject;

		GLuint modelLocation;
		GLuint solidColorLocation;

		vec4 objectColor;
		Material material;

};

#endif /* ___CONE_H___ */



