#ifndef CALLBACKS_H
#define	CALLBACKS_H

/** ICallbacks
* ICallbacks it a pure virtual class that specifies the interface that 
* must be implemented in order to recieve GLUT call backs through the glutBase
* functions. This allows a class to respond to glut events using class methods.
* 
* This class is suggested by Etay Meriri callback.h Copyright 2011 in OpenGL Step by 
* Step tutorials.
*
* @file callbacks.h
*/
class ICallbacks
{
public:
    
	/**
	* Responds to special key presses. Called by SpecialKeyboardCB in GLUTbase
	* which is registered with GLUT by calling glutSpecialFunc.
	* @param Key The generated ASCII character for the key whose press triggered the callback.
	* @param x The x-coordinate of the mouse relative to the window at the time the key is pressed. 
	* @param y The y-coordinate of the mouse relative to the window at the time the key is pressed.
	*/
	virtual void SpecialKeyboardCB(int Key, int x, int y) = 0;

	/**
	* Responds to regular key presses. Called by KeyboardCB in GLUTbase
	* which is registered with GLUT by calling glutSpecialFunc.
	* @param Key The generated ASCII character for the key whose press triggered the callback.
	* @param x The x-coordinate of the mouse relative to the window at the time the key is pressed. 
	* @param y The y-coordinate of the mouse relative to the window at the time the key is pressed.
	*/
    virtual void KeyboardCB(unsigned char Key, int x, int y) = 0;

	/**
	* Responds to mouse movement within the window without a button depressed. Called by 
	* PassiveMouseCB in GLUTbase which is registered with GLUT by calling glutPassiveMotionFunc.
	* @param x The x-coordinate of the mouse relative to the window. 
	* @param y The y-coordinate of the mouse relative to the window.
	*/
    virtual void PassiveMouseCB(int x, int y) = 0;

	/**
	* Acts as the display function for the window. Renders the entire scene. Called by 
	* RenderSceneCB in GLUTbase which is registered with GLUT by calling glutDisplayFunc.
	*/
    virtual void RenderSceneCB() = 0;

	/**
	* Called whenever the window it resphas and immediately before a windows 
	* first display. Called by ReshapeCB in GLUTBase which is register with GLUT by
	* calling glutReshapeFunc.
	* @windowWidth new window width in pixels.
	* @windowHeight new window height in pixels.
	*/
	virtual void ReshapeCB(int windowWidth, int windowHeight) = 0;

}; // end ICallbacks



#endif	/* CALLBACKS_H */

