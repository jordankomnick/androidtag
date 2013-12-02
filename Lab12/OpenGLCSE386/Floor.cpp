#include "Floor.h"


#include <vector>

#include "vertexStructs.h"

Floor::Floor(OpenGLApplicationBase * OpenGLApp, GLfloat w)
	:VisualObject( OpenGLApp ), sideLength( w )
{ }


void Floor::buildShaderProgram()
{
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "pVSpc.vert" },
		{ GL_FRAGMENT_SHADER, "pFSc.frag"},
		{ GL_NONE, NULL } // signals that there are no more shaders
	};

	// Read the files and create the OpenGL shader program.
	shaderProgram = LoadShaders(shaders);

	// This program will stay in effect for all draw calls until it is 
	// replaced with another or explicitly disabled (and the 
	// fixed function pipeline is enabld) by calling glUseProgram with NULL
    glUseProgram(shaderProgram);

	projectionLocation = glGetUniformLocation(shaderProgram, "ProjectionMatrix");
	assert(projectionLocation != 0xFFFFFFFF);

	modelViewLocation = glGetUniformLocation(shaderProgram, "ModelViewMatrix");
	assert(modelViewLocation != 0xFFFFFFFF);

} // end buildShaderProgram


// Construct visual object display list.
void Floor::initialize()
{
	float divisionsPerSide = sideLength;

	VertexData v0, v1, v2, v3;

	short color = 1;

	buildShaderProgram();

	GLuint VBO, IBO;

	glGenVertexArrays (1, &vertexArrayObject); 
	glBindVertexArray( vertexArrayObject );

	vector<VertexData> v;
	vector<unsigned int> indices;

	GLfloat tileWidth = sideLength / divisionsPerSide ;

	// Find corner of the board
	GLfloat tileX = -( (divisionsPerSide/2.0f)  * tileWidth);
	GLfloat tileZ = ( (divisionsPerSide/2.0f) * tileWidth);

	// define the two square colors
	vec4 color1( 0.9f, 0.9f, 0.9f, 1.0f );
 	vec4 color2( 0.05f, 0.05f, 0.05f, 1.0f );
	vec4 currentColor;
	GLuint currentIndex = 0;

	// Loop through rows
	for ( int j = 0 ; j < divisionsPerSide ; j++ ) {

		currentColor = (color++)%2 ? color1 : color2 ;

		// Loop through columns
		for ( int i = 0 ; i < divisionsPerSide  ; i++ ) {

			currentColor = (color++)%2 ? color1 : color2 ;

			v0 = VertexData ( vec3(tileX, 0.0f, tileZ-tileWidth ), currentColor );
			v.push_back( v0 );
			v1 = VertexData ( vec3(tileX, 0.0f, tileZ ), currentColor );
			v.push_back( v1 );

			tileX += tileWidth;

			v2 = VertexData ( vec3(tileX, 0.0f, tileZ), currentColor );
			v.push_back( v2 );
			v3 = VertexData ( vec3(tileX, 0.0f, tileZ-tileWidth), currentColor );
			v.push_back( v3 );

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 1);
			indices.push_back(currentIndex + 2);

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 2);
			indices.push_back(currentIndex + 3);
			
			currentIndex += 4;

		} // end for i

		tileX = - ( (divisionsPerSide/2.0f) * tileWidth);
		tileZ -= tileWidth;

	} // end for j

 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(VertexData), &v[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData),(const GLvoid*)sizeof(vec3));
	glEnableVertexAttribArray(2);

	numberOfIndices = indices.size(); 

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	v.clear();
	indices.clear();

	VisualObject::initialize();

} // end initialize


// Preform drawing operations
void Floor::draw()
{
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(OpenGLAPP->getProjectionMatrix()));
	glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, value_ptr(modelAndFixed));

	glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);

	// Draw all children
	VisualObject::draw();

} // end draw
