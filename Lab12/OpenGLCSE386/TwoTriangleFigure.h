#pragma once
#include "visualobject.h"

class TwoTriangleFigure : public VisualObject
{
	public:
		TwoTriangleFigure( OpenGLApplicationBase * OpenGLApp );
		~TwoTriangleFigure(void);

		void buildShaderProgram();

		virtual void initialize();

		virtual void draw();

	protected:

		GLuint gWorldLocation;
		GLuint VBO;
		GLuint CBO;
}; 


