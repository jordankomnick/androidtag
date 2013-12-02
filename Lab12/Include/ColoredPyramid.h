#ifndef ___COLORED_PYRAMID_H___
#define ___COLORED_PYRAMID_H___

#include "visualobject.h"
#include "LoadShaders.h"
#include "Lighting.h"

class ColoredPyramid : public VisualObject
{
	public:

		/**
		 * Constructor for the Pyramid class. Created a visual object in the shape of a pyramid. Vertex
		 * positions are expressed relative to an origin that is located in the center of the pyramid.
		 * @param OpenGLApp - reference to the OpenGLApplicationBase object that holds the projection
		 *        and viewing transformations
		 * @param w - width of the base of the pyramid. Default is 1 unit.
		 * @param h - height of the pyramid. Default is 1 unit.
		 */
		ColoredPyramid(OpenGLApplicationBase * OpenGLApp, GLfloat w = 1.0f, GLfloat h = 1.0f);

		/**
		 * Builds shader program and initializes vertex array object that will be used to render the 
		 * the object.
		 */
		virtual void initialize();

		/*
		 * Renders the object using the visual object using the vertex array object that was created by
		 * the initialize method.
		 */
		virtual void draw();

	protected:

		/**
		 * Builds the shader program.
		 */
		void buildShaderProgram();
		
		int numberOfIndices; // number of indices to be used during indexed rendering.

		GLfloat height; // height for the pyramid
		GLfloat width; // width for the pyramid
	
		// location in the shader of the projection matrix uniform variable
		GLuint projectionLocation; 

		// location in the shader of the model/view matrix uniform variable
		GLuint modelViewLocation;
		Material material;

};

#endif // ___COLORED_PYRAMID_H___