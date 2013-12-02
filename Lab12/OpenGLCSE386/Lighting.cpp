#include "Lighting.h"

GLint getUniformLocation(GLuint const m_shaderProg, char* pUniformName)
{
	glUseProgram( m_shaderProg);
    GLint Location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (Location == 0xFFFFFFFF)
    {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return Location;
}

vec3 findUnitNormal (vec3 v0, vec3 v1, vec3 v2)
{
	return normalize(cross(v1 - v0, v2 - v1));
}