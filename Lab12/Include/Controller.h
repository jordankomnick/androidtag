#ifndef ___CONTROLLER_H___
#define ___CONTROLLER_H___

#include "VisualObject.h"

// Base class for objects this manipulate properties of 
// VisaulObjects over time.
class Controller
{
public:

	// Allows VisualObject object to set the protected
	// target data member.
	friend class VisualObject;

	// Update method that is call by the Visual Object.
	virtual void update(float elapsedTimeSeconds);

protected:

	// Reference to the VisualObject that is being manipulated
	VisualObject* target;

}; // end Controller class

#endif /* ___CONTROLLER_H___ */