#include "Sphere.h"


#include <vector>
#include <math.h>

#include "vertexStructs.h"

#define M_PI 3.1415926535897932384626433832795f

Sphere::Sphere( OpenGLApplicationBase * OpenGLApp, float radius, int slices, int stacks, vec4 color )
	:VisualObject(OpenGLApp), radius(radius), slices(slices), stacks(stacks), objectColor(color)
{ 
	bodyIndicesCount = bottomIndicesCount = topIndicesCount = 0;
}


Sphere::~Sphere(void)
{
	glDeleteVertexArrays (1, &vertexArrayObjectForBottom);
	glDeleteVertexArrays (1, &vertexArrayObjectForTop);
}

void Sphere::buildShaderProgram()
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

	material = Material( getUniformLocation( shaderProgram, "object.ambientMat"),
						 getUniformLocation( shaderProgram, "object.diffuseMat"),
						 getUniformLocation( shaderProgram, "object.specularMat"),
						 getUniformLocation( shaderProgram, "object.specularExp"),
						 getUniformLocation( shaderProgram, "object.emissiveMat" ),
						 getUniformLocation( shaderProgram, "object.textureMapped") );
	
	//material.setAmbientAndDiffuseMat( objectColor );
	material.setAmbientAndDiffuseMat( vec4(1.0f) );
	material.setSpecularMat( vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	material.setSpecularExponentMat( 64.0f );
	material.setTextureMapped(true);
}


// Construct visual object display list.
void Sphere::initialize()
{
	textureObject = setupTexture( "wood1-Sphere1.bmp");
	//textureObject = setupTexture( "earth.bmp" );
	buildShaderProgram();

	stackStep = 2.0f * M_PI / (stacks*2.0f);
	sliceStep = 2.0f * M_PI / -slices;

	// Do the stacks
    z0 = 1.0;
    z1 = cos(stackStep);
    r0 = 0.0;
    r1 = sin(stackStep);

	initializeSphereTop();
	initializeSphereBody();
	initializeSphereBottom();
	
	VisualObject::initialize();

} // end initialize


// Construct visual object display list.
void Sphere::initializeSphereTop()
{
	// vector containers to hold  data
	vector<pntVertexData> v; // vertex positions
	vector<unsigned int> indices; // indices
	GLuint VBO, IBO; // Identifiers for buffer objects

	GLuint currentIndex = 0;

    vec3 normal = vec3( 0.0f, 0.0f, 1.0f );
	vec3 vTop = vec3( 0.0f, 0.0f, radius );
	float theta = 0.0f;

    for (int j = slices; j >  0; j--) {

		theta = j * sliceStep;
		vec3 n0 = vec3( cos(theta) * r1, sin(theta) * r1, z1 );
		vec3 v0 = vec3( cos(theta) * r1 * radius, sin(theta) * r1 *radius, z1* radius );

		v.push_back( pntVertexData( v0, n0, getSphericalTextCoords(theta, n0)) ); 
		indices.push_back(currentIndex++);

		theta = (j-1) * sliceStep;
		n0 = vec3( cos(theta) * r1, sin(theta) * r1, z1 );
		v0 = vec3( cos(theta) * r1 * radius, sin(theta) * r1 *radius, z1* radius );

		v.push_back( pntVertexData( v0, n0, getSphericalTextCoords(theta, n0)) ); 
		indices.push_back(currentIndex++);

		theta = j * sliceStep - sliceStep/2;
		v.push_back( pntVertexData( vTop, normal, getSphericalTextCoords(theta, normal)) ); 
		indices.push_back(currentIndex++);

	}

	glGenVertexArrays (1, &vertexArrayObjectForTop);
	glBindVertexArray( vertexArrayObjectForTop );

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

} // initializeSphereTop


// Construct visual object display list.
void Sphere::initializeSphereBody()
{

	// vector containers to hold  data
	vector<pntVertexData> v; // vertex positions
	vector<unsigned int> indices; // indices
	GLuint VBO, IBO; // Identifiers for buffer objects

	GLuint currentIndex = 0;
	float theta = 0.0f;

    for ( int i=1; i < stacks-1; i++)
    {
		z0 = z1; 
		z1 = cos((i+1)*stackStep);
        r0 = r1; 
		r1 = sin((i+1)*stackStep);

        for (int j=0; j < slices; j++ )
        {
			theta = j * sliceStep;
            vec3 n01 = normalize(vec3(cos(theta) * r1, sin(theta) * r1, z1 ));
            vec3 v0 = vec3(cos(theta) * r1 * radius, sin(theta) * r1 * radius, z1 * radius  );
			v.push_back( pntVertexData( v0, n01, getSphericalTextCoords(theta, n01 )) ); // 0
			
			vec3 n11 = normalize(vec3(cos(theta) * r0, sin(theta) * r0, z0 ));
			vec3 v1 = vec3(cos(theta) * r0 * radius, sin(theta) * r0 * radius, z0 * radius  );
			v.push_back( pntVertexData( v1, n11, getSphericalTextCoords(theta,n11)) ); // 1
			
			theta = (j+1) * sliceStep;
			vec3 n23 = normalize(vec3(cos(theta) * r1, sin(theta) * r1, z1 ));
			vec3 v2 = vec3(cos(theta) * r1* radius, sin(theta) * r1 * radius, z1 * radius  );
			v.push_back( pntVertexData( v2, n23, getSphericalTextCoords(theta, n23)) ); // 2

			vec3 n33 = normalize(vec3(cos(theta) * r0, sin(theta) * r0, z0 ));
            vec3 v3 = vec3(cos(theta) * r0 *radius, sin(theta) * r0 *radius, z0 * radius  );
			v.push_back( pntVertexData( v3, n33, getSphericalTextCoords(theta,n33)) ); // 3

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 3);
			indices.push_back(currentIndex + 2);

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 1);
			indices.push_back(currentIndex + 3);

			currentIndex += 4;
        }
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

} // end initializeSphereBody


// Construct visual object display list.
void Sphere::initializeSphereBottom()
{
	// vector containers to hold  data
	vector<pntVertexData> v; // vertex positions
	vector<unsigned int> indices; // indices
	GLuint VBO, IBO; // Identifiers for buffer objects

	GLuint currentIndex = 0;

	z0 = z1;
    r0 = r1;

	vec3 vBottom = vec3( 0.0f, 0.0f, -radius );
    vec3 normal = vec3( 0.0f, 0.0f, -1.0f );

	float theta = 0.0f;
    for (int j = 0; j < slices; j++) {

		theta = j * sliceStep;
		vec3 n0 = vec3( cos(theta) * r0, sin(theta) * r0, z1 );
		vec3 v0 = vec3( cos(theta) * r0 * radius, sin(theta) * r0 * radius, z0* radius );

		v.push_back( pntVertexData( v0, n0, getSphericalTextCoords(theta, n0)) ); 
		indices.push_back(currentIndex++);

		theta = (j+1) * sliceStep;
		n0 = vec3( cos(theta) * r0, sin(theta) * r0, z1 );
		v0 = vec3( cos(theta) * r0 * radius, sin(theta) * r0 * radius, z0* radius );

		v.push_back( pntVertexData( v0, n0, getSphericalTextCoords(theta, n0)) ); 
		indices.push_back(currentIndex++);

		theta = (j) * sliceStep + sliceStep/2;
		v.push_back( pntVertexData( vBottom, normal, getSphericalTextCoords(theta, normal)) ); 
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

} // end initializeSphereBottom


// Preform drawing operations
void Sphere::draw()
{
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelAndFixed));
	glUniform4fv(solidColorLocation, 1, value_ptr(objectColor));

	material.setShaderMaterialProperties();

	glBindTexture(GL_TEXTURE_2D, textureObject);

	// Draw top
	glBindVertexArray(vertexArrayObjectForTop);
    glDrawElements(GL_TRIANGLES, topIndicesCount, GL_UNSIGNED_INT, 0);

	// Draw body
	glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, bodyIndicesCount, GL_UNSIGNED_INT, 0);
	
	// Draw bottom
	glBindVertexArray(vertexArrayObjectForBottom);
    glDrawElements(GL_TRIANGLES, bottomIndicesCount, GL_UNSIGNED_INT, 0);

	// Draw all children
	VisualObject::draw();

} // end draw


