//#include <math.h>
//#define GLM_MESSAGES
//#define GLM_FORCE_RADIANS

#define GLM_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/swizzle.hpp>

using namespace glm;
 
#include "OpenGLApplicationBase.h"

#include "Sphere.h"
#include "Cylinder.h"
#include "Torus.h"
#include "Cone.h"
#include "Floor2.h"
#include "Wall.h"
#include "SpaceShip.h"
#include "Cube.h"
#include "SoundSource.h"

#include "Spinner.h"
#include "OrbitController.h"
#include "WaypointController2.h"

#include "FollowCamera.h"

#include "SharedGeneralLighting.h"
#include "SharedProjectionAndViewing.h"

#include "Framework.h"

void print(vec3 v) {

	cout << "[ " << v.x << " " << v.y << " " << v.z << " ]" << endl;
}

class bachmaerLab12 : public OpenGLApplicationBase
{
public:

	bachmaerLab12()
	{
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "vfixed-function-shared-proj-view.vert" },
			{ GL_FRAGMENT_SHADER, "pfixed-function-shared-lighting-phong.frag"},
			{ GL_NONE, NULL } // signals that there are no more shaders
		};

		// Read the files and create the OpenGL shader program.
		GLuint shaderProgram = setUpShaderProgram(shaders);
			
		setUpLighting();

		// *** Sound ***
		sound = new SoundSource("Footsteps.wav", this);
		root.addChild(sound);

		pyramid1 = new Sphere( this );
		pyramid1 ->fixedTransformation = rotate(mat4(1.0f), -90.0f, vec3(1,0,0));
		pyramid1->addController(new SpinnerController(vec3(3.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 15.0f));
		pyramid1->setShader(shaderProgram);
		root.addChild( pyramid1 );

		vec4 white = vec4(1.0f);
		pyramid2 = new Cylinder( this, 4.0f, 1.0f, white );
		pyramid2->addController(new SpinnerController(vec3(-3.0f, -2.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 15.0f));
		pyramid2->fixedTransformation = rotate(mat4(1.0f), -90.0f, vec3(1.0f,0.0f,0.0f)) ;
		pyramid2->setShader(shaderProgram);
		root.addChild( pyramid2 );

		pyramid3 = new Cone( this );
		pyramid3->addController( new OrbitController( 10.0f, vec3( 0.0f, 0.0f, 0.0f )));
		pyramid3->setShader(shaderProgram);
		root.addChild( pyramid3 );

		VisualObject * planet1 = new Sphere( this, 0.5, 16, 8, vec4(0.2f, 0.2f, 0.85f, 1.0f) );
		planet1->fixedTransformation = translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0));
		planet1->setShader(shaderProgram);
		pyramid3->addChild(planet1);

		// *** Uniform Blocks *** 
		shaderProgram = setUpShaderProgram(shaders);

		vector<vec3> waypoints;
		waypoints.push_back(vec3( 3.5f, -3.0f, 3.5f ));
		waypoints.push_back(vec3( -3.5f, -3.0f, -3.5f ));
		waypoints.push_back(vec3( 3.5f, -3.0f, -3.5f ));
		waypoints.push_back(vec3( -3.5f, -3.0f, 3.5f ));
		waypoints.push_back(vec3( 3.5f, 2.5f, 3.5f ));
		waypoints.push_back(vec3( -3.5f, 2.5f, -3.5f ));
		waypoints.push_back(vec3( 3.5f, 2.5f, -3.5f ));
		waypoints.push_back(vec3( -3.5f, 2.5f, 3.5f ));
		
		pyramid4 = new Cone( this );
		pyramid4->fixedTransformation = rotate( mat4(1.0f), -90.0f, vec3(1.0f, 0.0f, 0.0f) );
		WaypointController2 * wp = new WaypointController2(waypoints);
		pyramid4->addController( wp );
		pyramid4->setShader(shaderProgram);
		root.addChild( pyramid4 );

		// *** Sound ***
		followCamera = new FollowCamera();
		followCamera->cont = wp;
		root.addChild(followCamera);

		wall = new Wall(this, "brick.bmp");
		wall->fixedTransformation = translate(mat4(1.0f), vec3(0.0f, -3.0f, -4.0f));
		wall->setShader(shaderProgram);
		root.addChild(wall);

		floor = new Floor2(this);
		floor->setShader(shaderProgram);
		root.addChild( floor );

		// This object will be transparent. Add it last so that is will be rendered last.
		pyramid0 = new Cube( this, 2.0f, 2.0f, 2.0f, vec4( 0.2f, 0.2, 0.5f, 0.5f) ); // *** Alpha Blending ***
		pyramid0->addController(new SpinnerController(vec3(3.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 25.0f));
		pyramid0->setShader(shaderProgram);
		root.addChild( pyramid0 );

		rotationX = 0.0f;
		rotationY = 0.0f;
		viewTranslate = -12.0f;

		VisualObject root2;
		VisualObject *child1 = new VisualObject(this);
		VisualObject *child2 = new VisualObject(this);
		VisualObject *child3 = new VisualObject(this);

		child1->modelMatrix = translate(mat4(1.0f), vec3(10.0f, 0.0f, 0.0f)); 
		child2->modelMatrix = translate(mat4(1.0f), vec3(0.0f, 10.0f, 0.0f)) * rotate(mat4(1.0f), -90.0f, vec3(1,0,0)); 
		child3->modelMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 10.0f)); 

		root2.addChild(child1);
		child1->addChild(child2);
		child2->addChild(child3);

		print(child3->getWorldPosition());

		print((root.modelMatrix * child1->modelMatrix * child2->modelMatrix * child3->modelMatrix)[3].xyz);

	} // end constructor


	~bachmaerLab12()
	{

	} // end destructor


	// Read the files and create the OpenGL shader program.
	GLuint setUpShaderProgram( ShaderInfo shaders[] ) {

		GLuint shaderProgram = LoadShaders(shaders);
		 
		generalLighting.setUniformBlockForShader( shaderProgram );
		projectionAndViewing.setUniformBlockForShader(shaderProgram);

		return shaderProgram;
	}


	void setUpLighting() {
		
		// ***** Ambient Light **************
		generalLighting.setEnabled( GL_LIGHT_ZERO, true );
		generalLighting.setAmbientColor( GL_LIGHT_ZERO, vec4(0.2f, 0.2f, 0.2f, 1.0f));

		// *****  Directional Light ****************
		generalLighting.setEnabled( GL_LIGHT_ONE, true );
		generalLighting.setDiffuseColor( GL_LIGHT_ONE, vec4(0.75f, 0.75f, 0.75f, 1.0f) );
		generalLighting.setSpecularColor( GL_LIGHT_ONE, vec4(1.0f, 1.0f, 1.0f, 1.0f) );
		generalLighting.setPositionOrDirection( GL_LIGHT_ONE, vec4(1.0f, 1.0f, 1.0f, 0.0f) );

		// ***** Positional Light ***************
		generalLighting.setEnabled( GL_LIGHT_TWO, true );
		generalLighting.setDiffuseColor( GL_LIGHT_TWO, vec4(0.5f, 0.5f, 0.5f, 1.0f) );
		generalLighting.setSpecularColor( GL_LIGHT_TWO, vec4(1.0f, 1.0f, 1.0f, 1.0f) );
		generalLighting.setPositionOrDirection( GL_LIGHT_TWO, vec4(1.0f, 3.0f, 1.0f, 1.0f) );

		// ***** Spot Light **************
		generalLighting.setEnabled( GL_LIGHT_THREE, true );
		generalLighting.setIsSpot( GL_LIGHT_THREE, true );
		generalLighting.setDiffuseColor( GL_LIGHT_THREE, vec4(1.0f, 1.0f, 1.0f, 1.0f) );
		generalLighting.setSpecularColor( GL_LIGHT_THREE, vec4(1.0f, 1.0f, 1.0f, 1.0f) );
		generalLighting.setPositionOrDirection( GL_LIGHT_THREE, vec4(0.0f, 8.0f, 0.0f, 1.0f) );
		generalLighting.setSpotDirection( GL_LIGHT_THREE, vec3(0.0f, -1.0f, 0.0f) );
		generalLighting.setSpotCutoffCos( GL_LIGHT_THREE, cos(glm::radians(15.0f)) );

	} // end setUpLighting

    virtual bool initialize()
    {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// *** Fog ***
		//glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable( GL_DEPTH_TEST );

		setUpMenus();

		// Set the viewing transformation that will be used to render all objects 
		// in the scene.
		this->ViewMatrix = translate(mat4(1.0f), vec3( 0.0f, 0.0f,-12.0f ));

		root.initialize();

		floor->modelMatrix = translate(mat4(1.0f), vec3( 0.0f, -3.0f, 0.0f ));

		// *** Sound ***
		sound->setLooping(true);
		sound->play();

		return true;

    } // end initialize


	// Render scene objects
	virtual void drawScene()
	{ 
		root.draw();

	} // end drawScene


	// Update scene objects inbetween frames
	virtual void updateScene( float elapsedTimeSec ) 
	{ 
		setViewPoint();

		root.update( elapsedTimeSec );

	} // end updateScene


	// Set the viewing transformations
	void setViewPoint(  ) 
	{
		mat4 transView, rotateViewX, rotateViewY;
		transView = translate(mat4(1.0f), vec3( 0.0f, 0.0f, viewTranslate ));
		rotateViewX = rotate(mat4(1.0f), rotationX, vec3(1.0f, 0.0f, 0.0f)); 
		rotateViewY = rotate(mat4(1.0f), rotationY, vec3(0.0f, 1.0f, 0.0f)); 
				
		//ViewMatrix = transView * rotateViewX * rotateViewY;

		// *** Sound ***
		ViewMatrix = followCamera->getViewMatrix();

		projectionAndViewing.setViewMatrix(ViewMatrix);

	} // end setViewPoint


	void ReshapeCB(int windowWidth, int windowHeight)
	{
		OpenGLApplicationBase::ReshapeCB(windowWidth, windowHeight);

		projectionAndViewing.setProjectionMatrix(ProjectionMatrix);

	} // end ReshapeCB


	void SpecialKeyboardCB(int Key, int x, int y)
	{
		switch (Key) {

			case GLUT_KEY_RIGHT:
				rotationY++;
				break;
			case GLUT_KEY_LEFT:
				rotationY--;
				break;
			case GLUT_KEY_UP:
				rotationX++;
				break;
			case GLUT_KEY_DOWN:
				rotationX--;
				break;
			default:
				OpenGLApplicationBase::SpecialKeyboardCB(Key, x, y);
				break;
		}

	} // end SpecialKeyboardCB


	// Call back of letter keys and numbers
	void KeyboardCB(unsigned char Key, int x, int y)
	{
		static SpinnerController * pyramid1Cont;
		switch (Key) {
			case 'w': case 'W':
				viewTranslate++;
				break;
			case 's': case 'S':
				viewTranslate--;
				break; 
			case '1':
				if (pyramid0->getParent() == NULL ) {
					root.addChild(pyramid0);
				}
				else {
					pyramid0->detachFromParent();
				}
				break;
			case '2':
				cout << "case 2" << endl;
				if (pyramid1->hasController() ) {
					pyramid1Cont = (SpinnerController*)pyramid1->removeController();
					cout << "detach" << endl;
				}
				else {
					pyramid1->addController (pyramid1Cont);
					cout << "attach" << endl;
				}
				break;

			case 'a':

				if (generalLighting.getEnabled( GL_LIGHT_ZERO ) == true ) {
					generalLighting.setEnabled( GL_LIGHT_ZERO,  false );
				}
				else {
					generalLighting.setEnabled( GL_LIGHT_ZERO,  true );
				}

				break;
			case 'd':

				if (generalLighting.getEnabled( GL_LIGHT_ONE ) == true ) {
					generalLighting.setEnabled( GL_LIGHT_ONE,  false );
				}
				else {
					generalLighting.setEnabled( GL_LIGHT_ONE,  true );
				}
				break;
			case 'p':

				if (generalLighting.getEnabled( GL_LIGHT_TWO ) == true ) {
					generalLighting.setEnabled( GL_LIGHT_TWO,  false );
				}
				else {
					generalLighting.setEnabled( GL_LIGHT_TWO,  true );
				}
				break;
			case 'x':

				if (generalLighting.getEnabled( GL_LIGHT_THREE ) == true ) {
					generalLighting.setEnabled( GL_LIGHT_THREE,  false );
				}
				else {
					generalLighting.setEnabled( GL_LIGHT_THREE,  true );
				}
				break;

			default:
				OpenGLApplicationBase::KeyboardCB(Key, x, y);
					break;
		}

	} // end KeyboardCB


	friend void mainMenu(int value);

	void setUpMenus()
	{	
		GLuint menu2id = createPolygonMenu();
		GLuint menu3id = createFrontFaceMenu();
		GLuint menu4id = createPointSizeMenu();
		GLuint menu5id = createAntialiasingMenu();

		// Create main menu
		topMenu = glutCreateMenu(mainMenu);
		glutAddSubMenu("Polygon Mode", menu2id); 
		glutAddSubMenu("Rendered Polygon Face ", menu3id);
		glutAddSubMenu("Point Size ", menu4id); 
		glutAddSubMenu("Antialiasing ", menu5id);
		glutAddMenuEntry("Quit", 1); // Specify indentifier for "Quit"			  
		glutAttachMenu(GLUT_RIGHT_BUTTON); // Attach menu to right mouse button

	} //end setUpMenus

	// Data Members

	VisualObject * pyramid0, * pyramid1, *pyramid2, *pyramid3, *pyramid4, * floor, * wall;
	VisualObject * planet0, * planet1, * planet2;
	VisualObject root;

	GLfloat rotationX, rotationY, viewTranslate;

	SharedGeneralLighting generalLighting;
	SharedProjectionAndViewing projectionAndViewing;

	FollowCamera * followCamera;

	SoundSource * sound;

}; // end bachmaerLab12


// Global reference to the OpenGLApplicationBase class
// Can be used in friend functions to call methods and 
// access private data members.
bachmaerLab12 * openGLApplicationPtr;

void mainMenu(int value)
{
	switch (value) {

		case(1):
			glutLeaveMainLoop(); // End the program
			break;
		default:
			cout << "Unknown Main Menu Selection!" << endl;

	}

} // end mainMenu


int main(int argc, char** argv)
{
    GLUTBaseInit(argc, argv);

	GLUTBaseCreateWindow( "CSE 386 Lab 12" );

    bachmaerLab12 pApp;

	// Set global reference that friends= functions can use
	openGLApplicationPtr = &pApp;
	
	pApp.initialize();

	GLUTBaseRunApplication(&pApp);

    return 0;

} // end main