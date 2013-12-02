#include "Pyramid.h"

#include "vector"

Pyramid::Pyramid(OpenGLApplicationBase * OpenGLApp, GLfloat w, GLfloat h)
	:VisualObject(OpenGLApp), height(h), width(w)
{ }

void Pyramid::initialize()
{
	buildShaderProgram();
	glUseProgram(shaderProgram);

	GLuint VBO;
	GLuint CBO;
	GLuint IBO;

	glGenVertexArrays (1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	vec3 v0 = vec3(width/2, -height/2, width/2);
	vec3 v1 = vec3(width/2, -height/2, -width/2);
	vec3 v2 = vec3(-width/2, -height/2, -width/2);
	vec3 v3 = vec3(-width/2, -height/2, width/2);
	vec3 v4 = vec3(0.0f, height/2, 0.0f);

	vector<vec3> v;

	v.push_back(v0);
	v.push_back(v1);
	v.push_back(v2);
	v.push_back(v3);
	v.push_back(v4);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(vec3), &v[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	vector<vec4> c;

	vec4 c1 = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vec4 c2 = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	vec4 c3 = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	vec4 c4 = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	vec4 c5 = vec4(1.0f, 0.0f, 1.0f, 1.0f);

	c.push_back(c1);
	c.push_back(c2);
	c.push_back(c3);
	c.push_back(c4);
	c.push_back(c5);

	glGenBuffers(1, &CBO);
	glBindBuffer(GL_ARRAY_BUFFER, CBO);
	glBufferData(GL_ARRAY_BUFFER, c.size() * sizeof(vec4), &c[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	vector<GLuint> indices;

	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(3);
	indices.push_back(3);
	indices.push_back(4);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(4);
	indices.push_back(1);
	indices.push_back(1);
	indices.push_back(4);
	indices.push_back(0);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(0);

	numberOfIndices = indices.size();

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

} // end initialize

void Pyramid::buildShaderProgram()
{
	if ( shaderProgram==0 )  {

		// Create array of ShaderInfo structs that specifies the vertex and
		// fragment shaders to be compiled and linked into a program.
		ShaderInfo shaders[] = {{ GL_VERTEX_SHADER, "pVSpc.vert" },
		{ GL_FRAGMENT_SHADER, "pFSc.frag"},
		{ GL_NONE, NULL } // signals that there are no more shaders
		};

		// Read the files and create the OpenGL shader program.
		shaderProgram = LoadShaders(shaders);
	}

	projectionLocation = glGetUniformLocation(shaderProgram, "ProjectionMatrix");
	assert(projectionLocation != 0xFFFFFFFF);

	modelViewLocation = glGetUniformLocation(shaderProgram, "ModelViewMatrix");
	assert(modelViewLocation != 0xFFFFFFFF);

} // end buildShaderProgram

void Pyramid::draw()
{
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(OpenGLAPP->getProjectionMatrix()));
	glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, value_ptr(modelAndFixed));

	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);

	// Draw all children
	VisualObject::draw();
	
} // end draw

