#include "ColoredFigureOne.h"

#include <vector>

//ColoredFigureOne::ColoredFigureOne( OpenGLApplicationBase * OpenGLApp )
//	:VisualObject(OpenGLApp)
//{
//	
//
//} // end VisualObject


//ColoredFigureOne::~ColoredFigureOne(void)
//{
//
//
//}
void ColoredFigureOne::buildShaderProgram()
{
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "pVScolor.vert" },
		{ GL_FRAGMENT_SHADER, "pFScolor.frag"},
		{ GL_NONE, NULL } // signals that there are no more shaders
	};
	
	// Read the files and create the OpenGL shader program.
	shaderProgram = LoadShaders(shaders);

	// This program will stay in effect for all draw calls until it is 
	// replaced with another or explicitly disabled (and the 
	// fixed function pipeline is enabld) by calling glUseProgram with NULL
    glUseProgram(shaderProgram);

}


void ColoredFigureOne::initialize()
{
	buildShaderProgram();

	GLuint VBO;
	GLuint CBO;

	glGenVertexArrays (1, &vertexArrayObject); 
	glBindVertexArray( vertexArrayObject );

	vector<vec3> v;

    v.push_back( vec3( 0.0f,  0.0f, 0.0f) );
    v.push_back( vec3(0.0f, 0.5f, 0.0f) );
    v.push_back( vec3(0.0f, -0.5f, 0.0f) );
    v.push_back( vec3(0.5f, 0.0f, 0.0f) );


 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(vec3), &v[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	vector<vec4> c;

    c.push_back( vec4( 1.0f,  0.0f, 0.0f, 1.0f) );
    c.push_back( vec4(0.0f, 1.0f, 0.0f, 1.0f) );
    c.push_back( vec4(0.0f, 0.0f, 1.0f, 1.0f) );
    c.push_back( vec4(1.0f, 0.0f, 1.0f, 1.0f) );


 	glGenBuffers(1, &CBO);
	glBindBuffer(GL_ARRAY_BUFFER, CBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(vec4), &c[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

} // end initialize

void ColoredFigureOne::draw()
{
//	mat4 gWVP = OpenGLAPP->getProjectionMatrix() * OpenGLAPP->getViewMatrix();
	glUseProgram(shaderProgram);
	glBindVertexArray(vertexArrayObject);
	glDrawArrays(GL_POINTS, 0, 4);
	   
} // end draw

