#include "FigureOne.h"

#include <vector>

//FigureOne::FigureOne( OpenGLApplicationBase * OpenGLApp )
//	:VisualObject(OpenGLApp)
//{
//	
//
//} // end VisualObject
//
//
//FigureOne::~FigureOne(void)
//{
//
//
//}
void FigureOne::buildShaderProgram()
{
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "pVS.vert" },
		{ GL_FRAGMENT_SHADER, "pFS.frag"},
		{ GL_NONE, NULL } // signals that there are no more shaders
	};

	// Read the files and create the OpenGL shader program.
	shaderProgram = LoadShaders(shaders);

	// This program will stay in effect for all draw calls until it is 
	// replaced with another or explicitly disabled (and the 
	// fixed function pipeline is enabld) by calling glUseProgram with NULL
    glUseProgram(shaderProgram);

}


void FigureOne::initialize()
{
	buildShaderProgram();

	glUseProgram(shaderProgram);

	GLuint VBO;

	glGenVertexArrays (1, &vertexArrayObject); 
	glBindVertexArray( vertexArrayObject );

	vector<vec3> v;

    v.push_back( vec3( 0.0f,  0.0f, 0.0f) );
    v.push_back( vec3(0.0f, 0.5f, 0.0f) );
    v.push_back( vec3(0.0f, -0.5f, 0.0f) );
    v.push_back( vec3(0.5f, 0.0f, 0.0f) );
    v.push_back( vec3(-0.5f, 0.0f, 0.0f) );

 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(vec3), &v[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	cout << "FigureOne initilize method called." << endl;

} // end initialize

void FigureOne::draw()
{
//	mat4 gWVP = OpenGLAPP->getProjectionMatrix() * OpenGLAPP->getViewMatrix();

	glUseProgram(shaderProgram);
	glBindVertexArray(vertexArrayObject);
	glDrawArrays(GL_POINTS, 0, 5);



	   
} // end draw

