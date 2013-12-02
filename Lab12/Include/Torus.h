#ifndef ___TORUS_H___
#define ___TORUS_H___

#include "visualobject.h"
#include "LoadShaders.h"
#include "Lighting.h"


class Torus : public VisualObject
{
	public:
		Torus( OpenGLApplicationBase * OpenGLApp,  
			float innerRadius= 0.25f, float outerRadius = 1.0f, 
			vec4 color = vec4( 0.0f, 1.0f, 0.0f, 1.0f), 
			int sides = 8, int rings = 16);

		virtual ~Torus(void);
			
		virtual void initialize();

		virtual void draw();

	protected:
	
		void buildShaderProgram();
		
		GLdouble dInnerRadius, dOuterRadius;
		GLint nSides, nRings;

		GLuint modelLocation;
		GLuint solidColorLocation;

		int indicesCount;

		vec4 objectColor;
		Material material;
};

#endif /* ___TORUS_H___ */

