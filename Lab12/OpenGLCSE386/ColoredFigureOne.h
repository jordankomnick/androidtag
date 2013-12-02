
#pragma once
#include "visualobject.h"

class ColoredFigureOne : public VisualObject
{
	public:
		ColoredFigureOne( OpenGLApplicationBase * OpenGLApp );
		~ColoredFigureOne(void);

		void buildShaderProgram();

		virtual void initialize();

		virtual void draw();

	protected:
		
		GLuint gWorldLocation;
};


