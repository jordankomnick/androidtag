#ifndef ___FONT_H___
#define ___FONT_H___

#include <iostream>

using namespace std;

#include <string>

#include <GL/freeglut.h>

/**
 Outputs text in a designated screen position relative to the lower 
 left hand corner of the window. For the text to show up, it must be called 
 druing the rendering cycle inbetween clearing the color buffer and 
 swapping the front and back buffers. Do not call it during the update cycle.

 @param x horizontal position of the beginning of the text in pixels
 @param y vertical position of the beginning of the text in pixels
 @param text a string containg the text to be displayed in the window
 @param color The color in which the text is to be rendered
 @param font the bitmap font in which the text will be renderd. Allowable 
 choices are the following:
 
 GLUT_BITMAP_8_BY_13 - A variable-width font with every character fitting in a rectangle of 13 pixels high by at most 8 pixels wide.
 GLUT_BITMAP_9_BY_15 - A variable-width font with every character fitting in a rectangle of 15 pixels high by at most 9 pixels wide.
 GLUT_BITMAP_TIMES_ROMAN_10 - A 10-point variable-width Times Roman font.
 GLUT_BITMAP_TIMES_ROMAN_24 - A 24-point variable-width Times Roman font.
 GLUT_BITMAP_HELVETICA_10 - A 10-point variable-width Helvetica font.
 GLUT_BITMAP_HELVETICA_12 - A 12-point variable-width Helvetica font.
 GLUT_BITMAP_HELVETICA_18 - A 18-point variable-width Helvetica font.

 Example calls:

 root.draw();

 screenTextOutput(50, 100, "Hello world");

 screenTextOutput (50, 200, "Hello world again", vec4(1.0f, 0.0f, 0.0f, 1.0f));

 screenTextOutput (50, 300, "Hello world again and again", vec4(1.0f, 0.0f, 1.0f, 1.0f), GLUT_BITMAP_TIMES_ROMAN_24);

 Since it is using the fixed function pipeline, this is a bit of a hack. For rendering 
 Fonts using "Modern" OpenGL, see

 http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01

 It is a lot of work to get some letters on the screen!!
 */
void screenTextOutput(int x, int y, string text, vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f), void *font = GLUT_BITMAP_HELVETICA_18)
{
	GLint currentShader;

	// Retrive the identifier for the shader that is in use
	glGetIntegerv( GL_CURRENT_PROGRAM, &currentShader);

	// Unbind shader in use to enable fixed function pipeline functionality
	glUseProgram(0);

	// Store lighting mode
	GLboolean lightFlag = glIsEnabled(GL_LIGHTING); 

	// Turn lighting off for bitmap text
	if (lightFlag == GL_TRUE) {
		glDisable(GL_LIGHTING); 
	}

	// Set the Projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, glutGet( GLUT_WINDOW_WIDTH ), 0.0, glutGet( GLUT_WINDOW_HEIGHT) );
	
	// Clear the modelview stack
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();	

	// Set the color for the text
	glColor4f(color.x, color.y, color.z, color.w);
	
	// Set the position for the first letter in the string
	glRasterPos2i(x, y);

	// Render the text
	glutBitmapString(font, (const unsigned char*) text.c_str());

	// Restore the modelview stack
	glPopMatrix();

	// Restore the projection matrix stack
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Restore the lighting mode
	if (lightFlag == GL_TRUE) {
		glEnable(GL_LIGHTING); // Turn Lighting on
	}

	// Restore shader program
	glUseProgram( currentShader );

}// end screenTextOutput

#endif // ___FONT_H___