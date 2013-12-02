//#include <math.h>
//#define GLM_MESSAGES
//#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

#include "OpenGLApplicationBase.h"

#include "FigureOne.h"
#include "ColoredFigureOne.h"
#include "TwoTriangleFigure.h"


static GLint figure = 1;

class CSE386LabOne : public OpenGLApplicationBase
{
public:

	CSE386LabOne()
	{
		fig1 = new FigureOne( this );
		cfig1 = new ColoredFigureOne( this );
		ttf = new TwoTriangleFigure( this );
	}


    virtual bool initialize()
    {
		glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		setUpMenus();

		ProjectionMatrix = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
		ViewMatrix = translate(mat4(1.0f), vec3( 0.0f, 0.0f, -1.0f));

		fig1->initialize();
		cfig1->initialize();
		ttf->initialize();

		return true;

    } // end initialize

	// Render scene objects
	virtual void drawScene()
	{ 
		switch( figure ) {

			case 1:
				fig1->draw();
				break;
			case 2:
				cfig1->draw();
				break;
			case 3:
				ttf->draw();
				break;
			default:
				cout << "Invalid figure selected." << endl;
				break;
		}

	} // end drawScene

	// Update scene objects inbetween frames
	virtual void updateScene( float elapsedTimeSec )
	{ 

	} // end updateScene

	friend void figureMenu(int value);

	GLuint createFigureMenu() 
	{
		// Create polygon submenu
		GLuint menuId = glutCreateMenu(figureMenu);
		// Specify menu items and their integer indentifiers
		glutAddMenuEntry("Figure 1", 1);
		glutAddMenuEntry("Figure 2", 2);
		glutAddMenuEntry("Figure 3", 3);
		glutAddMenuEntry("Figure 4", 4);
		glutAddMenuEntry("Figure 5", 5);
		glutAddMenuEntry("Figure 6", 6);
		glutAddMenuEntry("Figure 7", 7);
		glutAddMenuEntry("Figure 8", 8);
		glutAddMenuEntry("Figure 9", 9);

		return menuId;

} // end createFigureMenu

	friend void pointSizeMenu(int value);

	GLuint createPointSizeMenu() 
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

	friend void lineWidthMenu(int value);

	GLuint createLineWidthMenu() 
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


	friend void antiAliasingMenu(int value);

	GLuint createAntialiasingMenu() 
	{
		// Create polygon submenu
		GLuint menuId = glutCreateMenu(antiAliasingMenu);
		// Specify menu items and their integer indentifiers
		glutAddMenuEntry("On", 1);
		glutAddMenuEntry("Off", 2);

		return menuId;

} // end createPointSizeMenu


	
	friend void mainMenu(int value);

	void setUpMenus()
	{	
		// Create polygon submenu
		GLuint menu1id = createPolygonMenu();

		GLuint menu2id = createFrontFaceMenu();

		GLuint menu3id = createFigureMenu();

		GLuint menu4id = createPointSizeMenu();

		GLuint menu5id = createAntialiasingMenu();

		GLuint menu6id = createLineWidthMenu();

		// Create main menu
		topMenu = glutCreateMenu(mainMenu);
		glutAddSubMenu("Polygon Mode", menu1id); // Attach polygon Menu
		glutAddSubMenu("Rendered Polygon Face ", menu2id);	// Attach the polygon face Menu
		glutAddSubMenu("Rendered Figure ", menu3id);	// Attach the polygon face Menu
		glutAddSubMenu("Point Size ", menu4id);	// Attach the polygon face Menu
		glutAddSubMenu("Line Width ", menu6id);	// Attach the polygon face Menu
		glutAddSubMenu("Antialiasing ", menu5id);	// Attach the polygon face Menu
		glutAddMenuEntry("Quit", 1); // Specify indentifier for "Quit"			  
		glutAttachMenu(GLUT_RIGHT_BUTTON); // Attach menu to right mouse button

	} //end setUpMenus

	FigureOne* fig1;
	ColoredFigureOne* cfig1;
	TwoTriangleFigure* ttf;

}; // end CSE386LabOne


void figureMenu(int value)
{
	figure = value;
	cout << "Drawing figure " << value << endl;

} // end figureMenu

//void pointSizeMenu(int value)
//{
//	glPointSize( (GLfloat)value );
//	cout << "Point Size is " << value << " pixels." << endl;
//
//} // end figureMenu
//
//
//void lineWidthMenu(int value)
//{
//	glLineWidth( (GLfloat)value );
//	cout << "Line width is " << value << " pixels." << endl;
//
//} // end lineWidthMenu

//void antiAliasingMenu(int value)
//{
//	switch (value) {
//
//		case(1):
//			glEnable( GL_POINT_SMOOTH );
//			glEnable( GL_LINE_SMOOTH );
//			glEnable( GL_BLEND );
//			break;
//		case(2):
//			glDisable( GL_POINT_SMOOTH );
//			glDisable( GL_LINE_SMOOTH );
//			glDisable( GL_BLEND );
//			break;
//		default:
//			cout << "Unknown Main Menu Selection!" << endl;
//	}
//
//} // end figureMenu

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

	GLUTBaseCreateWindow( "CSE 386 Lab1" );

    CSE386LabOne pApp;
	
	pApp.initialize();

	GLUTBaseRunApplication(&pApp);

    return 0;

} // end main