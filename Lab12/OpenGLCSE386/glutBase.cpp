
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "glutbase.h"

// Points to the object implementing the ICallbacks interface which was delivered to
// GLUTBackendRun(). All events are forwarded to this object.
static ICallbacks* s_pCallbacks = NULL;




// *************** EVENT HANDLERS ***************************************

/**
* Responds to special key presses. 
* @param Key The generated ASCII character for the key whose press triggered the callback.
* @param x The x-coordinate of the mouse relative to the window at the time the key is pressed. 
* @param y The y-coordinate of the mouse relative to the window at the time the key is pressed.
*/
static void SpecialKeyboardCB(int Key, int x, int y)
{
    s_pCallbacks->SpecialKeyboardCB(Key, x, y);

} // end SpecialKeyboardCB

/**
* Responds to regular key presses. 
* @param Key The generated ASCII character for the key whose press triggered the callback.
* @param x The x-coordinate of the mouse relative to the window at the time the key is pressed. 
* @param y The y-coordinate of the mouse relative to the window at the time the key is pressed.
*/
static void KeyboardCB(unsigned char Key, int x, int y)
{
	s_pCallbacks->KeyboardCB(Key, x, y);

} // end KeyboardCB


/** 
* Function: Handle mouse movment events when no buttons are depressed.
*
* @param x mouse horizontal position
* @param y mouse vertical position 
*/
static void PassiveMouseCB(int x, int y)
{
    s_pCallbacks->PassiveMouseCB(x, y);

} // end PassiveMouseCB

/**
* Acts as the display function for the window. 
*/
static void RenderSceneCB()
{
    s_pCallbacks->RenderSceneCB(); // Redraw the scene

} // end RenderSceneCB

/**
* Registered as an idle function. Repeatedly called by GLUT when not 
* responding to other events in order to drive the animation loop and 
* get the scene rendered repeatedly.
*/
void IdleCB()
{
    glutPostRedisplay(); // Mark the current window as needing to be redisplayed

} // end IdleCB


/** 
* Function: Handle changes in window visibility status. Animation is disabled while
* window is not visible.
* @param status visibility of the window
*/
static void VisibilityCB(int status)
{
	if (status == GLUT_VISIBLE) {
		glutIdleFunc(IdleCB); // Turn on animation
	}
	else {
		glutIdleFunc(NULL);	// Turn off animation when the window is not visible
	}

} // end VisibilityCB

/**
* Function: Handle changes in window size. Called when the window is 
* first opened and whenever the window is moved or resized.
* @param windowWidth - new window width in pixels, 
* @param windowHeight - new window hieght in pixels
*/
static void ReshapeCB(int windowWidth, int windowHeight)
{
    s_pCallbacks->ReshapeCB( windowWidth, windowHeight);

} // end ReshapeCB


/** 
* Function: Disables animation when a menu is exposed.
* @param status state of the menu
* @param statusx - horizontal position of the mouse
* @param statusy - vertical position of the mouse
*/
static void menuStatusCB(int status, int x, int y)
{
	if (status == GLUT_MENU_IN_USE) {
		glutIdleFunc(NULL);			// Turn off animation
	}
	else {
		glutIdleFunc(IdleCB);		// Turn on animation
	}

} // end menuStatusCB

// *************** END EVENT HANDLERS ***************************************

/**
* Function: Register callback functions to handle window and global
* events. These functions are called by glut to handle the particular
* types of event for which they are registered. The code provides 
* examples of registering event hander functions for
* many different types of events. 
*/
static void registerCallBacks()
{
    glutDisplayFunc(RenderSceneCB);		// callback for window redisplay
    glutIdleFunc(IdleCB);				// idle callback
    glutSpecialFunc(SpecialKeyboardCB);	// callback for special keystrokes
    glutPassiveMotionFunc(PassiveMouseCB); // callback for mouse motion with no buttons depressed.
    glutKeyboardFunc(KeyboardCB);		// callback for ascii character input
	glutVisibilityFunc(VisibilityCB);	// callback for visibility changes
	glutReshapeFunc(ReshapeCB);		// callback for window size changes
	glutMenuStatusFunc(menuStatusCB);	// callback for menu exposures

} // end registerCallBacks


/** 
Function: Displays OpengGL and GLSL version information.
*/
static void displayOpenGlInfo(void)
{
	// Display  the company responsible for this GL implementation
	fprintf( stdout, "OpenGL INFO:\n\tOpenGL Implementor: %s\n", glGetString(GL_VENDOR) );

	// Display the renderer/graphics card
	fprintf( stdout, "\tRenderer: %s\n", glGetString(GL_RENDERER) );

	// Display the OpenGL version that is implemented
	fprintf( stdout, "\tOpenGL Version: %s\n", glGetString(GL_VERSION) );

	// Display the GLSL version
	fprintf( stdout, "\tGLSL Version: %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION) );

} // end displayOpenGlInfo


/*
* Performs basic initialization for an OpenGL GLUT application. 
*/
void GLUTBaseInit(int argc, char** argv)
{
	// Pass any applicable command line arguments to GLUT. These arguments
	// are platform dependent.
    glutInit(&argc, argv);

	// Set initial display mode for double buffering and RGBA color
	// Explicitly request double buffering, RGBA (Red Green Blue Alpha) color mode and 
	// a depth buffer for hidden surface removal.
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	// Indicates to GLUT that the flow of control should return to the program after
	// the window is closed and the GLUTmain loop is exited.
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

} // end GLUTBaseInit


/*
* Specifies  initial title, size, position and names of window and whethere or not fullscreen is desired. 
*/
GLboolean GLUTBaseCreateWindow(const char* pTitle, GLuint width, GLuint height, GLboolean fullScreen )
{
	// Specify window size.
	glutInitWindowSize(width, height);

	// Specify window placement relative to the upper left hand corner of the screen.
	glutInitWindowPosition(100, 100);

	// Create a window using the string contained in pTitle and 
	// make it the current window. glutCreateWindow does return a 
	// unique integer identifier for the created window. Here the 
	// identifier is not save under the assumption that there will only 
	// be one window.
	glutCreateWindow(pTitle);

	// Check if fullScreen is set to true 
	if (fullScreen)
	{
		glutFullScreen(); // Go to full screen rendering mode.
	}

	// GLEW does not entirely support the Core GLUT Profile out of the box. 
	// The following statement fixes the problem.
	glewExperimental = GL_TRUE;

	// Intilize GLEW. This must be done after glut is initialized.
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return false; // GLEW could not be initialized.
	}

	// Display OpenGL and GLSL version information to the console
	displayOpenGlInfo();

	return true; // window was successfully created.

} // end GLUTBaseCreateWindow


/*
* Takes an object of a class that implements the ICallbacks interface
* and registers it handle GLUT generated events, then enters the GLUT main
* loop.
*/
void GLUTBaseRunApplication(ICallbacks* pCallbacks)
{
	// Check to make sure an appropriate Object has been supplied.
	// The object must be a member of a class that implments the 
	// ICallbacks interface.
    if (!pCallbacks)
    {
        fprintf(stderr, "%s : callbacks not specified!\n", __FUNCTION__);
        return;
    }

	// Save a reference to the ICallbacks object
    s_pCallbacks = pCallbacks;
    
	// Register the ICallbacks to receive events generated by GLUT.
	registerCallBacks();
     
	// Enter the GLUT main loop. Control will not return until the
	// window is closed.
    glutMainLoop();

	// Keeps the console window open after the main loop has been exited
	system( "pause" ); 

} // end GLUTBaseRun