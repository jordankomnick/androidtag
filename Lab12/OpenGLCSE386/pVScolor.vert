// Targeting version 3.3 of GLSL. If the compiler does not support 
// 3.3 it will cause an error.
#version 150

// Declares that a vertex specific attributes which is a vector of 3 floats
// will be known as 'Position' in the shader. 'Vertex specific' means that 
// for every invocation of the shader in the GPU the value of a new vertex 
// from the buffer will be supplied. The first section of the statement, 
// layout (location = 0), creates the binding between the attribute name 
// and attribute in the buffer. This is required for cases where the vertex 
// contains several attributes (position, normal, texture coordinates, etc).

in vec3 vertexPosition; 
in vec4 vertexColor;

out vec4 vertexColor0;

// There can only be one main function for each shader stage
void main()
{
	// 'gl_Position' is a special built in variable that is supposed to 
	// contain the homogeneous (containing X, Y, Z and W components) of the vertex 
	// position. The rasterizer will look for that variable and will use it 
	// as the position in screen space (following a few more transformations).
    gl_Position = vec4(vertexPosition, 1.0f);

	vertexColor0 = vertexColor;

}