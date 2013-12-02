#include "Floor2.h"

#include <vector>

#include "vertexStructs.h"

Floor2::Floor2(OpenGLApplicationBase * OpenGLApp, GLfloat w)
	:VisualObject( OpenGLApp ), sideLength( w ), normalVector(vec3(0.0f, 1.0f, 0.0f))
{ 
		divisionsPerSide = (int)sideLength;

		if( divisionsPerSide% 2 != 0) {
			divisionsPerSide = divisionsPerSide + 1;
		}
		tileWidth = sideLength / divisionsPerSide ;
}

Floor2::~Floor2(void)
{
	glDeleteVertexArrays (1, &lightSquareVertexArrayObject);

}

void Floor2::buildShaderProgram()
{
	if ( shaderProgram==0 )  {

		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "pVSpSolidColor.vert" },
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

	modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	assert(modelLocation != 0xFFFFFFFF);

	solidColorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	//assert(solidColorLocation != 0xFFFFFFFF);

	darkMaterial = Material( getUniformLocation( shaderProgram, "object.ambientMat"),
						 getUniformLocation( shaderProgram, "object.diffuseMat"),
						 getUniformLocation( shaderProgram, "object.specularMat"),
						 getUniformLocation( shaderProgram, "object.specularExp"),
						 getUniformLocation( shaderProgram, "object.emissiveMat" ),
						 getUniformLocation( shaderProgram, "object.textureMapped") );
	
	darkMaterial.setAmbientAndDiffuseMat( vec4( 0.10f, 0.10f, 0.10f, 1.0f ) );
	darkMaterial.setSpecularMat( vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	darkMaterial.setSpecularExponentMat( 64.0f );

	lightMaterial = Material( getUniformLocation( shaderProgram, "object.ambientMat"),
						 getUniformLocation( shaderProgram, "object.diffuseMat"),
						 getUniformLocation( shaderProgram, "object.specularMat"),
						 getUniformLocation( shaderProgram, "object.specularExp"),
						 getUniformLocation( shaderProgram, "object.emissiveMat" ) ,
						 getUniformLocation( shaderProgram, "object.textureMapped") );
	
	lightMaterial.setAmbientAndDiffuseMat( vec4( 0.90f, 0.90f, 0.90f, 1.0f ) );
	lightMaterial.setSpecularMat( vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	lightMaterial.setSpecularExponentMat( 64.0f );

}


// Construct visual object display list.
void Floor2::initialize()
{
	buildShaderProgram();

	initilizeDarkSquares();

	initilizeLightSquares();

	VisualObject::initialize();

} // end initialize

void Floor2::initilizeDarkSquares() 
{
	pnVertexData v0, v1, v2, v3;

	GLuint VBO, IBO;

	glGenVertexArrays (1, &vertexArrayObject); 
	glBindVertexArray( vertexArrayObject );

	vector<pnVertexData> v;
	vector<unsigned int> indices;

	// Find corner of the board
	GLfloat tileX = -( (divisionsPerSide/2.0f)  * tileWidth ) ;
	GLfloat tileZ = ( (divisionsPerSide/2.0f) * tileWidth);

	GLuint currentIndex = 0;

	// Loop through rows
	for ( int j = 0 ; j < divisionsPerSide ; j++) {

		// Loop through columns
		for ( int i = 0 ; i < divisionsPerSide/2  ; i++ ) {

			v0 = pnVertexData ( vec3(tileX, 0.0f, tileZ-tileWidth ), normalVector );
			v.push_back( v0 );
			v1 = pnVertexData ( vec3(tileX, 0.0f, tileZ ), normalVector );
			v.push_back( v1 );

			tileX += tileWidth;

			v2 = pnVertexData ( vec3(tileX, 0.0f, tileZ), normalVector );
			v.push_back( v2 );
			v3 = pnVertexData ( vec3(tileX, 0.0f, tileZ-tileWidth), normalVector );
			v.push_back( v3 );

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 1);
			indices.push_back(currentIndex + 2);

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 2);
			indices.push_back(currentIndex + 3);
			
			currentIndex += 4;

			tileX += tileWidth;

		} // end for i

		if(j%2==0) {
			tileX = - ( (divisionsPerSide/2.0f) * tileWidth) + tileWidth ;
		}
		else {
			tileX = - ( (divisionsPerSide/2.0f) * tileWidth);
		}

		tileZ -= tileWidth;

	} // end for j

 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pnVertexData), &v[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pnVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pnVertexData),(const GLvoid*)sizeof(vec3));
	glEnableVertexAttribArray(1);

	darkSquareIndices = indices.size(); 

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	v.clear();
	indices.clear();

} // end initilizeDarkSquares


void Floor2::initilizeLightSquares() 
{
	pnVertexData v0, v1, v2, v3;

	GLuint VBO, IBO;

	glGenVertexArrays (1, &lightSquareVertexArrayObject); 
	glBindVertexArray( lightSquareVertexArrayObject );

	vector<pnVertexData> v;
	vector<unsigned int> indices;

	// Find corner of the board
	GLfloat tileX = -( (divisionsPerSide/2.0f)  * tileWidth) + tileWidth;
	GLfloat tileZ = ( (divisionsPerSide/2.0f) * tileWidth);

	GLuint currentIndex = 0;

	// Loop through rows
	for ( int j = 0 ; j < divisionsPerSide ; j++) {

		// Loop through columns
		for ( int i = 0 ; i < divisionsPerSide/2  ; i++ ) {

			v0 = pnVertexData ( vec3(tileX, 0.0f, tileZ-tileWidth ), normalVector );
			v.push_back( v0 );
			v1 = pnVertexData ( vec3(tileX, 0.0f, tileZ ), normalVector );
			v.push_back( v1 );

			tileX += tileWidth;

			v2 = pnVertexData ( vec3(tileX, 0.0f, tileZ), normalVector );
			v.push_back( v2 );
			v3 = pnVertexData ( vec3(tileX, 0.0f, tileZ-tileWidth), normalVector );
			v.push_back( v3 );

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 1);
			indices.push_back(currentIndex + 2);

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 2);
			indices.push_back(currentIndex + 3);
			
			currentIndex += 4;

			tileX += tileWidth;

		} // end for i

		if(j%2==0) {
			tileX = - ( (divisionsPerSide/2.0f) * tileWidth);
		}
		else {
			tileX = - ( (divisionsPerSide/2.0f) * tileWidth) + tileWidth;
		}

		tileZ -= tileWidth;

	} // end for j

 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pnVertexData), &v[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pnVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pnVertexData),(const GLvoid*)sizeof(vec3));
	glEnableVertexAttribArray(1);

	lightSquareIndices = indices.size(); 

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	v.clear();
	indices.clear();

} // end initilizeLightSquares


// Preform drawing operations
void Floor2::draw()
{
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelAndFixed));

	glBindVertexArray(vertexArrayObject);
	glUniform4fv(solidColorLocation, 1, value_ptr(vec4( 0.10f, 0.10f, 0.10f, 1.0f )));
	darkMaterial.setShaderMaterialProperties();
    glDrawElements(GL_TRIANGLES, darkSquareIndices, GL_UNSIGNED_INT, 0);

	glBindVertexArray(lightSquareVertexArrayObject);
	glUniform4fv(solidColorLocation, 1, value_ptr(vec4 ( 0.9f, 0.9f, 0.9f, 1.0f )));
	lightMaterial.setShaderMaterialProperties();
    glDrawElements(GL_TRIANGLES, lightSquareIndices, GL_UNSIGNED_INT, 0);

	// Draw all children
	VisualObject::draw();

} // end draw


