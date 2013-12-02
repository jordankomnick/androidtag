#ifndef OPENGL_APPLICATION_BASE_H
#define	OPENGL_APPLICATION_BASE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/swizzle.hpp>
#include <glm/gtc/constants.hpp>

using namespace glm;

#include "callbacks.h"
#include "glutbase.h"

const GLint FPS = 60; // Desired maximum number of frames per second

/**
* Super class for OpenGL applications. Implements the ICallbacks interface
* so that it can respond to events raised by the GLUTbase framework. initialize,
* drawScene, and updateScene are pure virtual and must be overriden. 
*/
class OpenGLApplicationBase : public ICallbacks
{
public:

	/**
	* Initiliaze the application. Load vertex data and set initial transforms.
	*/
	virtual bool initialize() = 0;

	/**
	* Render scene objects
	*/
	virtual void drawScene() = 0;

	/**
	* Update scene objects inbetween frames based on the amount time since
	* the scene was last rendered.
	*/
	virtual void updateScene( float elapsedTimeSec ) = 0;

	/**
	* Checks the OpenGL state machine errors. Prints out a message and clears the error state.
	* It is called once per render update cycle by default, but could be called more often
	*/
	void checkOpenGLErrors(void);

	/**
	* Responds to special key presses. 
	* @param Key The generated ASCII character for the key whose press triggered the callback.
	* @param x The x-coordinate of the mouse relative to the window at the time the key is pressed. 
	* @param y The y-coordinate of the mouse relative to the window at the time the key is pressed.
	*/
	virtual void SpecialKeyboardCB(int Key, int x, int y) override;

	/**
	* Responds to regular key presses. 
	* @param Key The generated ASCII character for the key whose press triggered the callback.
	* @param x The x-coordinate of the mouse relative to the window at the time the key is pressed. 
	* @param y The y-coordinate of the mouse relative to the window at the time the key is pressed.
	*/
    virtual void KeyboardCB(unsigned char Key, int x, int y) override;

	/**
	* Responds to mouse movement within the window without a button depressed. 
	* @param x The x-coordinate of the mouse relative to the window. 
	* @param y The y-coordinate of the mouse relative to the window.
	*/
    virtual void PassiveMouseCB(int x, int y) override;

	/**
	* Acts as the display function for the window. Renders and updates the entire scene each
	* time it is callded as long as sufficient time has pass since the last rendering.
	*
	* THIS METHOD SHOULD NOT BE OVERRIDEN.
	*/
    void RenderSceneCB() override;

	/**
	* Called whenever the window it resphapes and immediately before a windows 
	* first display. 
	* @windowWidth new window width in pixels.
	* @windowHeight new window height in pixels.
	*/
	virtual void ReshapeCB( int windowWidth, int windowHeight ) override;

	mat4 getProjectionMatrix() { return ProjectionMatrix; };
	mat4 getViewMatrix() { return ViewMatrix; };

protected:

	friend void polygonMenu(int value);

	GLuint createPolygonMenu();

	friend void frontFaceMenu(int value);

	GLuint 	createFrontFaceMenu();

	friend void pointSizeMenu(int value);

	GLuint createPointSizeMenu();

	friend void lineWidthMenu(int value);

	GLuint createLineWidthMenu();

	friend void antiAliasingMenu(int value);

	GLuint createAntialiasingMenu();


	// milliseconds between frames. Used to control the framerate.
	static const GLint FRAME_INTERVAL = 1000/FPS; 

	GLuint topMenu; 

public:

	mat4 ProjectionMatrix;
	mat4 ViewMatrix;

}; // end OpenGLApplicationBase

extern ICallbacks* s_pCallbacks;

#endif /* OPENGL_APPLICATION_BASE */

