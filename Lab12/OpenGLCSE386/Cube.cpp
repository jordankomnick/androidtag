#include "Cube.h"

#include <vector>

#include "vertexStructs.h"

Cube::Cube(OpenGLApplicationBase * OpenGLApp, GLfloat w, GLfloat h, GLfloat d, vec4 c)
	:VisualObject( OpenGLApp ),width(w), height(h), depth(d), color(c)
{
}

void Cube::buildShaders()
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

	glUseProgram(shaderProgram);

	modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	assert(modelLocation != 0xFFFFFFFF);

	solidColorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	material = Material( getUniformLocation( shaderProgram, "object.ambientMat"),
						 getUniformLocation( shaderProgram, "object.diffuseMat"),
						 getUniformLocation( shaderProgram, "object.specularMat"),
						 getUniformLocation( shaderProgram, "object.specularExp"),
						 getUniformLocation( shaderProgram, "object.emissiveMat" ),
						 getUniformLocation( shaderProgram, "object.textureMapped") );
	
	material.setAmbientAndDiffuseMat( color );
	material.setSpecularMat( vec4(0.2f, 0.2f, 0.2f, 0.0f) );
	material.setSpecularExponentMat( 64.0f );

} // end buildShaders


void Cube::initialize()
{
	GLfloat hW = width/2.0f;
	GLfloat hH = height/2.0f;
	GLfloat hD = depth/2.0f;

	buildShaders();

	vector<pnVertexData> v;

	vec3 v0 = vec3( -hW, -hH, hD);
	vec3 v1 =  vec3( -hW, -hH, -hD);
	vec3 v2 = vec3( hW, -hH, -hD);
	vec3 v3 = vec3( hW, -hH, hD);
	vec3 v4 = vec3( -hW, hH, hD);
	vec3 v5 = vec3( -hW, hH, -hD);
	vec3 v6 = vec3( hW, hH, -hD);
	vec3 v7 = vec3(hW, hH, hD);

	vec3 normal;

	// 0 4 1
	// 1 4 5
	// 3 2 6
	// 3 6 7
	// 0 3 7
	// 0 7 4
	// 1 5 2
	// 2 5 6
	// 4 7 5
	// 4 6 5
	// 0 2 3
	// 0 1 2

	normal = findUnitNormal(v0, v4, v1);
	v.push_back(pnVertexData(v0, normal));
	v.push_back(pnVertexData(v4, normal));
	v.push_back(pnVertexData(v1, normal));

	normal = findUnitNormal(v1, v4, v5);
	v.push_back(pnVertexData(v1, normal));
	v.push_back(pnVertexData(v4, normal));
	v.push_back(pnVertexData(v5, normal));

	normal = findUnitNormal(v3, v2, v6);
	v.push_back(pnVertexData(v3, normal));
	v.push_back(pnVertexData(v2, normal));
	v.push_back(pnVertexData(v6, normal));

	normal = findUnitNormal(v3, v6, v7);
	v.push_back(pnVertexData(v3, normal));
	v.push_back(pnVertexData(v6, normal));
	v.push_back(pnVertexData(v7, normal));

	normal = findUnitNormal(v0, v3, v7);
	v.push_back(pnVertexData(v0, normal));
	v.push_back(pnVertexData(v3, normal));
	v.push_back(pnVertexData(v7, normal));

	normal = findUnitNormal(v0, v7, v4);
	v.push_back(pnVertexData(v0, normal));
	v.push_back(pnVertexData(v7, normal));
	v.push_back(pnVertexData(v4, normal));

	normal = findUnitNormal(v1, v5, v2);
	v.push_back(pnVertexData(v1, normal));
	v.push_back(pnVertexData(v5, normal));
	v.push_back(pnVertexData(v2, normal));

	normal = findUnitNormal(v2, v5, v6);
	v.push_back(pnVertexData(v2, normal));
	v.push_back(pnVertexData(v5, normal));
	v.push_back(pnVertexData(v6, normal));

	normal = findUnitNormal(v4, v7, v6);
	v.push_back(pnVertexData(v4, normal));
	v.push_back(pnVertexData(v7, normal));
	v.push_back(pnVertexData(v6, normal));

	normal = findUnitNormal(v4, v6, v5);
	v.push_back(pnVertexData(v4, normal));
	v.push_back(pnVertexData(v6, normal));
	v.push_back(pnVertexData(v5, normal));

	normal = findUnitNormal(v0, v2, v3);
	v.push_back(pnVertexData(v0, normal));
	v.push_back(pnVertexData(v2, normal));
	v.push_back(pnVertexData(v3, normal));

	normal = findUnitNormal(v0, v1, v2);
	v.push_back(pnVertexData(v0, normal));
	v.push_back(pnVertexData(v1, normal));
	v.push_back(pnVertexData(v2, normal));

	GLuint VBO;

	glGenVertexArrays (1, &vertexArrayObject); 
	glBindVertexArray( vertexArrayObject );

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pnVertexData), &v[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pnVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pnVertexData), (const GLvoid*)sizeof(vec3));
	glEnableVertexAttribArray(1);

	numberOfIndices = v.size();
	v.clear();

	VisualObject::initialize();

} // end initialize

// Preform drawing operations
void Cube::draw()
{
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelAndFixed));

	material.setShaderMaterialProperties();

	glBindVertexArray(vertexArrayObject);
	
	// *** Alpha Blending ***
	glEnable (GL_BLEND);
    glDepthMask (GL_FALSE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glDrawArrays(GL_TRIANGLES, 0, numberOfIndices );

	// *** Alpha Blending ***
    glDepthMask (GL_TRUE);
    glDisable (GL_BLEND);

	// Draw all children
	VisualObject::draw();

} // end draw

