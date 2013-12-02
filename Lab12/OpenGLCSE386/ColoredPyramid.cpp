#include "ColoredPyramid.h"

#include <vector>

#include "vertexStructs.h"

ColoredPyramid::ColoredPyramid(OpenGLApplicationBase * OpenGLApp, GLfloat w, GLfloat h)
	:VisualObject( OpenGLApp ), height(h), width(w)
{ }

void ColoredPyramid::buildShaderProgram()
{
	if ( shaderProgram==0 )  {

		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "pVSpc.vert" },
			{ GL_FRAGMENT_SHADER, "pFSc.frag"},
			{ GL_NONE, NULL } // signals that there are no more shaders
		};

		// Read the files and create the OpenGL shader program.
		shaderProgram = LoadShaders(shaders);
	}

	// This program will stay in effect for all draw calls until it is 
	// replaced with another or explicitly disabled (and the 
	// fixed function pipeline is enabld) by calling glUseProgram with NULL
    glUseProgram(shaderProgram);

	projectionLocation = glGetUniformLocation(shaderProgram, "ProjectionMatrix");
	assert(projectionLocation != 0xFFFFFFFF);

	modelViewLocation = glGetUniformLocation(shaderProgram, "ModelViewMatrix");
	assert(modelViewLocation != 0xFFFFFFFF);

} // end buildShaderProgram


void ColoredPyramid::initialize()
{
	GLfloat hH = height/2.0f;
	GLfloat hW = width/2.0f;

	buildShaderProgram();

	GLuint VBO, IBO;

	glGenVertexArrays (1, &vertexArrayObject); 
	glBindVertexArray( vertexArrayObject );

	vector<VertexData> v;

    v.push_back( VertexData( vec3( 0.0f,  hH, 0.0f), vec4( 1.0f, 0.0f, 0.0f, 1.0f )));
    v.push_back( VertexData( vec3(-hW, -hH, hW), vec4( 0.0f, 1.0f, 0.0f, 1.0f )));
    v.push_back( VertexData( vec3( hW, -hH, hW), vec4( 0.0f, 0.0f, 1.0f, 1.0f )));
    v.push_back( VertexData( vec3( hW, -hH, -hW), vec4( 1.0f, 1.0f, 0.0f, 1.0f )));
    v.push_back( VertexData( vec3(-hW, -hH, -hW), vec4( 0.0f, 1.0f, 1.0f, 1.0f )));

 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(VertexData), &v[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData),(const GLvoid*)sizeof(vec3));
	glEnableVertexAttribArray(2);

	vector<unsigned int> i;
	i.push_back(0);
	i.push_back(1);
	i.push_back(2);

	i.push_back(0);
	i.push_back(2);
	i.push_back(3);

	i.push_back(0);
	i.push_back(3);
	i.push_back(4);

	i.push_back(0);
	i.push_back(4);
	i.push_back(1);

	i.push_back(4);
	i.push_back(2);
	i.push_back(1);

	i.push_back(4);
	i.push_back(3);
	i.push_back(2);

	numberOfIndices = sizeof( i );

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i.size() * sizeof(unsigned int), &i[0], GL_STATIC_DRAW);

} // end initialize

// Preform drawing operations
void ColoredPyramid::draw()
{
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(OpenGLAPP->getProjectionMatrix()));
	glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, value_ptr(modelAndFixed));

	glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);

	// Draw all children
	VisualObject::draw();

} // end draw

