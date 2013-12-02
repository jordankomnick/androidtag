#ifndef ___WALL_H___
#define ___WALL_H___


#include "visualobject.h"
#include "LoadShaders.h"
#include "Lighting.h"

class Wall : public VisualObject
{
	public:
		Wall( OpenGLApplicationBase * OpenGLApp, string textureFileName = NULL,
			  float height = 6.0f, float width = 8.0f, 
			  int rows = 1, int columns = 1 );

		virtual ~Wall(void);
			
		virtual void initialize();

		virtual void draw();

	protected:
	
		void buildShaderProgram();

		string textureFileName;

		int rows, columns;
		float height, width;

		int indiceCount;
		GLuint textureObject;

		GLuint modelLocation;

		vec4 objectColor;
		Material material;

};

#endif // ___WALL_H___

