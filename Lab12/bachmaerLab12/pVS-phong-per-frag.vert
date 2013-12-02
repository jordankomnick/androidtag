// Targeting version 1.5 of GLSL. If the compiler does not support 
// 1.5 it will cause an error.
#version 150

// Declare default precision - eliminates warnings
precision mediump float;

// Declares that a vertex specific attribute which is a vector of 3 floats
// will be known as 'Position' in the shader. 'Vertex specific' means that 
// for every invocation of the shader in the GPU the value of a new vertex 
// from the buffer will be supplied. The first section of the statement, 
// layout (location = 0), creates the binding between the attribute name 
// and attribute in the buffer. This is required for cases where the vertex 
// contains several attributes (position, normal, texture coordinates, etc).

in vec3 vertexPosition; 
in vec3 vertexNormal; 
in vec2 vertexTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec2 TexCoord0;
out vec3 Normal0;
out vec3 WorldPos0;  

// There can only be one main function for each shader stage
void main()
{
	// Calculate position in world coordinates
	WorldPos0 = (modelMatrix * vec4(vertexPosition, 1.0f)).xyz;

	TexCoord0 = vertexTexCoord;

	// Calculate normal in world coordinates
    Normal0 = normalize(mat3(modelMatrix) * vertexNormal);

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0f);
}