#ifndef ___FLOOR_TWO_H___
#define ___FLOOR_TWO_H___

#include "visualobject.h"
#include "LoadShaders.h"
#include "Lighting.h"

class Floor2 : public VisualObject
{
	public:

		Floor2(OpenGLApplicationBase * OpenGLApp, GLfloat w = 8.0f );
		virtual ~Floor2(void);

		virtual void initialize();

		virtual void draw();

	protected:

		void buildShaderProgram();

		void initilizeDarkSquares();
		void initilizeLightSquares();
		
		int darkSquareIndices;
		int lightSquareIndices;
		GLuint lightSquareVertexArrayObject; 

		GLfloat sideLength; 
		int divisionsPerSide;
		GLfloat tileWidth;

		GLuint modelLocation;
		GLuint solidColorLocation;
		
		Material darkMaterial;
		Material lightMaterial;

		const vec3 normalVector;

};

#endif // ___FLOOR_TWO_H___
