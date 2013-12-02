#ifndef ___SPACESHIP_H___
#define ___SPACESHIP_H___

#include "visualobject.h"
#include "LoadShaders.h"

class SpaceShip : public VisualObject
{
	public:
		SpaceShip( OpenGLApplicationBase * OpenGLApp);

		virtual void setShader( GLuint shaderProgram );

	protected:

		GLuint projectionLocation; 
		GLuint modelViewLocation;
};

#endif // ___SPACESHIP_H___
