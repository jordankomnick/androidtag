#include "OpenGLApplicationBase.h"

// Points to the object implementing the ICallbacks interface which was delivered to
// GLUTBackendRun(). All events are forwarded to this object.
static ICallbacks* s_pCallbacks = NULL;


// *************** EVENT HANDLERS ***************************************

/**
* Default response to special key presses.
*/
void OpenGLApplicationBase::SpecialKeyboardCB(int Key, int x, int y)
{

} // end SpecialKeyboardCB
 

/**
* Default response to regular key presses.
*/
void OpenGLApplicationBase::KeyboardCB(unsigned char Key, int x, int y)
{
	switch (Key) {
		case 'q': case 27:
			glutLeaveMainLoop();
			break;
		case 'f': 
			glutFullScreenToggle();
			break;
    }

} // end KeyboardCB

/**
* Default response to passive mouse motion.
*/
void OpenGLApplicationBase::PassiveMouseCB(int x, int y)
{
	// cout << "mouse motion with button up at (" << x << ", " << y << ")" << endl;

} // end PassiveMouseCB


/**
* Default response to window resize and move events.
*/
void OpenGLApplicationBase::ReshapeCB(int windowWidth, int windowHeight)
{
	cout << "Window sized to " << windowWidth << " x " << windowHeight << endl;

	// Set viewport to entire client area of the window
	glViewport(0, 0, windowWidth, windowHeight);

	// Set up a perspective projection matrix with a 45 degree vertical FOV.
	// Aspect ratio equals that of the rendering window.
	// Sets the near and far clipping planes at point one and one hundred units. 
	ProjectionMatrix = perspective(45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 100.f);

} // end ReshapeCB

/*
* Render and update the scene
*/
void OpenGLApplicationBase::RenderSceneCB()
{
	// time in milliseconds of last frame render
	static GLint lastRenderTime = 0; 

	int currentTime = glutGet( GLUT_ELAPSED_TIME ); // Get current time

	int elapsedTime = currentTime - lastRenderTime; // Calc time since last frame

	// Check if enough time has elapsed since the last render.
	if (   elapsedTime >= FRAME_INTERVAL) {

		// Save time for this frame render
		lastRenderTime = currentTime; 

		// Clear the color and depth buffers
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// Draw the scene objects
		drawScene();

		// Flush all drawing commands and swapbuffers
		glutSwapBuffers();

		// Update the scene for the next frame
		updateScene( (float)elapsedTime / 1000.0f );
		
		// Query OpenGL for errors.
		checkOpenGLErrors();
	}

} // end RenderSceneCB

// *************** END EVENT HANDLERS ***************************************

/*
* Display messages to the console regarding any error that occured since the
* last render cycle.
*/
void OpenGLApplicationBase::checkOpenGLErrors(void)
{
	GLenum errorCode; // Current error code.
	const GLubyte *errorString; // Desriptive string for the error. 
	
	// glGetError returns the value of the error flag. No  errors are 
	// recorded until glGetError is called, the error code is returned, 
	// and the flag is reset to GL_NO_ERROR. If a call to glGetError 
	// returns GL_NO_ERROR, there has been no detectable error since the 
	// last call to glGetError, or since the GL was initialized.
	if (( errorCode = glGetError() ) != GL_NO_ERROR ) {

		errorString = gluErrorString( errorCode );
		cout << errorString << endl;
	}

} // end checkOpenGLErrors


GLuint OpenGLApplicationBase::createPolygonMenu() 
{

	// Create polygon submenu
	GLuint menuId = glutCreateMenu(polygonMenu);
	// Specify menu items and their integer indentifiers
	glutAddMenuEntry("Filled", 1);
	glutAddMenuEntry("Outline", 2);
	glutAddMenuEntry("Points", 3);

	return menuId;

} // end createPolygonMenu

GLuint 	OpenGLApplicationBase::createFrontFaceMenu() 
{
	// Create lighting submenu
	GLuint menuID = glutCreateMenu( frontFaceMenu );
	// Specify menu items and their integer indentifiers
	glutAddMenuEntry("CCW Only", 1);
	glutAddMenuEntry("CW Only", 2);
	glutAddMenuEntry("CCW and CW", 3);

	return menuID;

} // end createFrontFaceMenu

GLuint OpenGLApplicationBase::createPointSizeMenu() 
{
	// Create polygon submenu
	GLuint menuId = glutCreateMenu(pointSizeMenu);
	// Specify menu items and their integer indentifiers
	glutAddMenuEntry("One", 1);
	glutAddMenuEntry("Ten", 10);
	glutAddMenuEntry("Fifty", 50);
	glutAddMenuEntry("One Hundred", 100);
	glutAddMenuEntry("Five Hundered", 500);

	return menuId;

} // end createPointSizeMenu


GLuint OpenGLApplicationBase::createLineWidthMenu() 
{
	// Create polygon submenu
	GLuint menuId = glutCreateMenu(lineWidthMenu);
	// Specify menu items and their integer indentifiers
	glutAddMenuEntry("One", 1);
	glutAddMenuEntry("Ten", 10);
	glutAddMenuEntry("Twenty", 20);
	glutAddMenuEntry("Fifty", 50);

	return menuId;

} // end createLineWidthMenu


GLuint OpenGLApplicationBase::createAntialiasingMenu() 
{
	// Create polygon submenu
	GLuint menuId = glutCreateMenu(antiAliasingMenu);
	// Specify menu items and their integer indentifiers
	glutAddMenuEntry("On", 1);
	glutAddMenuEntry("Off", 2);

	return menuId;

} // end createPointSizeMenu

//******************** Menu Functions *********************************

// Function: Handle polygon menu calls
void polygonMenu(int value)
{
	switch (value) {

		case(1):
			// Render polygon fronts and fill them
			glPolygonMode(GL_FRONT, GL_FILL);
			break;
		case(2):
			// Render polygon fronts in wire frame
			glPolygonMode(GL_FRONT, GL_LINE);
			break;
		case(3):
			// Render polygon fronts in wire frame
			glPolygonMode(GL_FRONT, GL_POINT);
			break;
		default:
			cout << "Unknown Polygon Mode!" << endl;
	}

	// Signal GLUT to call display callback
	glutPostRedisplay();

} // end polygonMenu

void frontFaceMenu(int value)
{
	switch (value) {

		case(1):
			glFrontFace(GL_CCW);
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
			break;
		case(2):
			glFrontFace(GL_CW);
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
			break;
		case(3):
			glDisable(GL_CULL_FACE);
			break;
		default:
			cout << "Unknown Lighting Mode!" << endl;
	}

	// Signal GLUT to call display callback
	glutPostRedisplay();

} // end frontFaceMenu

void pointSizeMenu(int value)
{
	glPointSize( (GLfloat)value );
	cout << "Point Size is " << value << " pixels." << endl;

} // end figureMenu


void lineWidthMenu(int value)
{
	glLineWidth( (GLfloat)value );
	cout << "Line width is " << value << " pixels." << endl;

} // end lineWidthMenu

void antiAliasingMenu(int value)
{
	switch (value) {

		case(1):
			glEnable( GL_POINT_SMOOTH );
			glEnable( GL_LINE_SMOOTH );
			glEnable( GL_BLEND );
			break;
		case(2):
			glDisable( GL_POINT_SMOOTH );
			glDisable( GL_LINE_SMOOTH );
			glDisable( GL_BLEND );
			break;
		default:
			cout << "Unknown Main Menu Selection!" << endl;
	}

} // end figureMenu




//*********************** End Menu Functions **************************




