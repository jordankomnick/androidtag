#pragma once
#include "visualobject.h"

class FigureOne : public VisualObject
{
	public:
		FigureOne( OpenGLApplicationBase * OpenGLApp );
		~FigureOne(void);

		void buildShaderProgram();

		virtual void initialize();

		virtual void draw();

	protected:

		GLuint gWorldLocation;
};

