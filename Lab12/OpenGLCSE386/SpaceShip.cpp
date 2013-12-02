#include "SpaceShip.h"

#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"


SpaceShip::SpaceShip( OpenGLApplicationBase * OpenGLApp)
	: VisualObject( OpenGLApp )
{
		VisualObject * fuselage = new Cylinder(OpenGLApp, 4.0f);
		fuselage->fixedTransformation = translate(mat4(1.0f), vec3(0.0f, 0.0f, -2.0f));
		this->addChild( fuselage );

		VisualObject * nose = new Cone(OpenGLApp);
		nose->fixedTransformation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 2.0f));
		this->addChild(nose);

		VisualObject * engine = new Cone(OpenGLApp);
		engine->fixedTransformation = translate(mat4(1.0f), vec3(1.0f, 0.0f, -2.0f));
		this->addChild(engine);

		VisualObject * engine2 = new Cone(OpenGLApp);
		engine2->fixedTransformation = translate(mat4(1.0f), vec3(-1.0f, 0.0f, -2.0f));
		this->addChild(engine2);

		VisualObject * bubble = new Sphere(OpenGLApp, 0.5f);
		bubble->fixedTransformation = translate(mat4(1.0f), vec3(0.0f, 0.5f, 0.0f));
		this->addChild(bubble);

} // end SpaceShip constructor

void SpaceShip::setShader( GLuint shaderProgram ) 
{ 
	for (unsigned int i = 0; i < children.size(); i++) {

		children[i]->setShader(shaderProgram);
	}
}



