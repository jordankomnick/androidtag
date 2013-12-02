#ifndef ___FLOOR_H___
#define ___FLOOR_H___

#include "visualobject.h"
#include "LoadShaders.h"


class Floor : public VisualObject
{
	public:

		Floor(OpenGLApplicationBase * OpenGLApp, GLfloat w = 8.0f );

		virtual void initialize();

		virtual void draw();

	protected:

		void buildShaderProgram();
		
		int numberOfIndices;

		GLfloat sideLength;

		GLuint projectionLocation; 
		GLuint modelViewLocation;

};

#endif // ___FLOOR_H___
