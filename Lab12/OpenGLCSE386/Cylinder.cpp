#include "Cylinder.h"

#include <vector>
#include <math.h>

#include "vertexStructs.h"

#define M_PI 3.1415926535897932384626433832795f

Cylinder::Cylinder( OpenGLApplicationBase * OpenGLApp, float height, float radius, vec4 color, int stacks, int slices)
	:VisualObject(OpenGLApp),stacks(stacks), slices(slices), height(height), radius(radius) , objectColor(color)
{}

Cylinder::~Cylinder(void)
{
	glDeleteVertexArrays (1, &vertexArrayObjectForBottom);
	glDeleteVertexArrays (1, &vertexArrayObjectForTop);
}

void Cylinder::buildShaderProgram()
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
	// fixed function pipeline is enabled) by calling glUseProgram with NULL
    glUseProgram(shaderProgram);

	modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	assert(modelLocation != 0xFFFFFFFF);

	solidColorLocation = glGetUniformLocation(shaderProgram, "objectColor");
//	assert(solidColorLocation != 0xFFFFFFFF);

	material = Material( getUniformLocation( shaderProgram, "object.ambientMat"),
						 getUniformLocation( shaderProgram, "object.diffuseMat"),
						 getUniformLocation( shaderProgram, "object.specularMat"),
						 getUniformLocation( shaderProgram, "object.specularExp"),
						 getUniformLocation( shaderProgram, "object.emissiveMat" ),
						 getUniformLocation( shaderProgram, "object.textureMapped") );
	
	material.setAmbientAndDiffuseMat( vec4(1.0f) );
	material.setSpecularMat( vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	material.setSpecularExponentMat( 64.0f );

	material.setTextureMapped(true);
}





// Construct visual object display list.
void Cylinder::initialize()
{
	textureObject = setupTexture( "test.bmp" );

	buildShaderProgram();
	
	initializeCylinderBottom();
	initializeCylinderBody();
	initializeCylinderTop();

	VisualObject::initialize();

} // end initialize

void Cylinder::initializeCylinderBody()
{
    // Step in z and radius as stacks are drawn.
    double z0,z1;
    const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );
	float angleStep = 2.0f * M_PI / slices;

	// vector containers to hold  data
	vector<pntVertexData> v; // vertex positions
	vector<unsigned int> indices; // indices
	GLuint VBO, IBO; // Identifiers for buffer objects

	// Do the stacks */
	z0 = 0.0;
    z1 = zStep;
	GLuint currentIndex = 0;

	float theta = 0.0f;

    for ( int i=1; i<=stacks; i++)
    {
        if (i==stacks) {
            z1 = height;
		}

        for (int j=0; j < slices; j++ ) {

			theta = j * angleStep;

            vec3 n01 = vec3(cos(theta), sin(theta), 0.0 );     
			vec3 v0 = vec3(cos(theta)*radius, sin(theta)*radius, z0  );
			v.push_back( pntVertexData( v0, n01, getCylindericalTextCoords( theta, v0, height)) ); // 0
			
			vec3 v1 = vec3(cos(theta)*radius, sin(theta)*radius, z1  );
			v.push_back( pntVertexData( v1, n01, getCylindericalTextCoords( theta, v1, height )) ); // 1

			theta = (j+1)* angleStep;
			vec3 n23 = vec3(cos(theta), sin(theta), 0.0 );

			vec3 v2 = vec3(cos(theta)*radius, sin(theta)*radius, z0  );
			v.push_back( pntVertexData( v2, n23, getCylindericalTextCoords( theta, v2, height ) ) ); // 2

            vec3 v3 = vec3(cos(theta)*radius, sin(theta)*radius, z1  );
			v.push_back( pntVertexData( v3, n23, getCylindericalTextCoords( theta, v3, height ) ) ); // 3

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 2);
			indices.push_back(currentIndex + 3);

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 3);
			indices.push_back(currentIndex + 1);

			currentIndex += 4;
        }

        z0 = z1; 
		z1 += zStep;
    }

	glGenVertexArrays (1, &vertexArrayObject);
	glBindVertexArray( vertexArrayObject );

	// finally, create the buffer to hold interleaved  and bind the data to them
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Buffer for vertex data
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pntVertexData), &v[0], GL_STATIC_DRAW); //Buffering vertex data

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)sizeof(vec3) );
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)(2 * sizeof(vec3)) );
	glEnableVertexAttribArray(3);

	// Generate a buffer for the indices
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);

	// store the number of indices for later use
	bodyIndicesCount = indices.size();
 
	v.clear();
	indices.clear();

} // end initializeCylinderBody

void Cylinder::initializeCylinderBottom()
{
	float angleStep = 2.0f * M_PI / slices;

	// vector containers to hold  data
	vector<pntVertexData> v; // vertex positions
	vector<unsigned int> indices; // indices
	GLuint VBO, IBO; // Identifiers for buffer objects

	GLuint currentIndex = 0;

    vec3 normal = vec3(0.0, 0.0, -1.0 );
	vec3 bottomCtr = vec3(0.0, 0.0,  0.0 );

	float theta = 0.0f;
    for (int j = slices; j >=  0; j--) {

		theta = j * angleStep;
		vec3 v0 = vec3( cos(theta) * radius, sin(theta)*radius, 0.0f );
		v.push_back( pntVertexData( v0, normal, getCylindericalTextCoords( theta, v0, height)) ); 
		indices.push_back(currentIndex++);

		theta = (j-1) * angleStep;
		v0 = vec3( cos(theta) * radius, sin(theta)*radius, 0.0f );
		v.push_back( pntVertexData( v0, normal, getCylindericalTextCoords( theta, v0, height)) ); 
		indices.push_back(currentIndex++);

		theta = j * angleStep - angleStep/2;
		v.push_back( pntVertexData( bottomCtr, normal, getCylindericalTextCoords( theta, bottomCtr, height)) ); 
		indices.push_back(currentIndex++);

	}

	glGenVertexArrays (1, &vertexArrayObjectForBottom);
	glBindVertexArray( vertexArrayObjectForBottom );

	// finally, create the buffer to hold interleaved  and bind the data to them
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Buffer for vertex data
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pntVertexData), &v[0], GL_STATIC_DRAW); //Buffering vertex data

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)sizeof(vec3) );
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)(2 * sizeof(vec3)) );
	glEnableVertexAttribArray(3);

	// Generate a buffer for the indices
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);

	// store the number of indices for later use
	bottomIndicesCount = indices.size();

	v.clear();
	indices.clear();

} // end initializeCylinderBottom

void Cylinder::initializeCylinderTop()
{
	float angleStep = 2.0f * M_PI / slices; 

	// vector containers to hold  data
	vector<pntVertexData> v; // vertex positions
	vector<unsigned int> indices; // indices
	GLuint VBO, IBO; // Identifiers for buffer objects

	GLuint currentIndex = 0;

    vec3 normal = vec3(0.0, 0.0, 1.0 );
	vec3 topCtr = vec3(0.0, 0.0,  height );

	float theta = 0.0f;

    for (int j = 0; j <= slices; j++) {

		theta = j * angleStep;
		vec3 v0 = vec3( cos(theta) * radius, sin(theta)*radius, height );
		v.push_back( pntVertexData( v0, normal, getCylindericalTextCoords( theta, v0, height)) ); 
		indices.push_back(currentIndex++);

		theta = (j+1) * angleStep;
		v0 = vec3( cos(theta) * radius, sin(theta)*radius, height );
		v.push_back( pntVertexData( v0, normal, getCylindericalTextCoords( theta, v0, height)) ); 
		indices.push_back(currentIndex++);

		theta = (j) * angleStep + angleStep/2;
		v.push_back( pntVertexData( topCtr, normal, getCylindericalTextCoords( theta, topCtr, height) )); 
		indices.push_back(currentIndex++);
	}

	glGenVertexArrays (1, &vertexArrayObjectForTop);
	glBindVertexArray( vertexArrayObjectForTop);

	// finally, create the buffer to hold interleaved  and bind the data to them
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Buffer for vertex data
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pntVertexData), &v[0], GL_STATIC_DRAW); //Buffering vertex data

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)sizeof(vec3) );
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)(2 * sizeof(vec3)) );
	glEnableVertexAttribArray(3);

	// Generate a buffer for the indices
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);

	// store the number of indices for later use
	topIndicesCount = indices.size();

	v.clear();
	indices.clear();

	VisualObject::initialize();

} // end initializeCylinderTop

// Preform drawing operations
void Cylinder::draw()
{
	glUseProgram(shaderProgram);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelAndFixed));
	glUniform4fv(solidColorLocation, 1, value_ptr(objectColor));

	material.setTextureMapped(true);
	material.setShaderMaterialProperties();
	glBindTexture(GL_TEXTURE_2D, textureObject);

	// Draw body
	glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, bodyIndicesCount, GL_UNSIGNED_INT, 0);

	// Draw bottom
	glBindVertexArray(vertexArrayObjectForBottom);
    glDrawElements(GL_TRIANGLES, bottomIndicesCount, GL_UNSIGNED_INT, 0);

	// Draw top
	glBindVertexArray(vertexArrayObjectForTop);
    glDrawElements( GL_TRIANGLES, topIndicesCount, GL_UNSIGNED_INT, 0);

	// Draw all children
	VisualObject::draw();

} // end draw
