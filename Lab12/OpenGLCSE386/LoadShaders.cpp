//////////////////////////////////////////////////////////////////////////////
//
//  --- LoadShaders.cxx ---
//
//////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>

//#define GLEW_STATIC // STATIC BUILDS ARE NOT SUPPORTED

#include <GL/glew.h>
#include "LoadShaders.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//----------------------------------------------------------------------------

// Reads in the source code of a shader program.  
static const GLchar*
ReadShader( const char* filename )
{
#ifdef WIN32
	FILE* infile;
	fopen_s( &infile, filename, "rb" );
#else
    FILE* infile = fopen( filename, "rb" );
#endif // WIN32

    if ( !infile ) {
#ifdef _DEBUG
        std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
        return NULL;
    }
	else {

		std::cout << "Reading '" << filename << "' shader source code." << std::endl;
	}

    fseek( infile, 0, SEEK_END );
    int len = ftell( infile );
    fseek( infile, 0, SEEK_SET );

    GLchar* source = new GLchar[len+1];

    fread( source, 1, len, infile );
    fclose( infile );

    source[len] = 0;

    return const_cast<const GLchar*>(source);
}

//----------------------------------------------------------------------------

GLuint
LoadShaders( ShaderInfo* shaders )
{
    if ( shaders == NULL ) { return 0; }

	// Creates an empty Shader Program object and returns an unsigned int by which
	// it can be referenced. Shader objects will be attached to the program
	// object.
    GLuint program = glCreateProgram();

	// Declare and array of structures to hold information particular shaders.
    ShaderInfo* entry = shaders;

	// Loop though all shaders specified in entry array until a GL_NONE
	// is encountered in the type field. All shaders will be attached
	// to the shader program.
    while ( entry->type != GL_NONE ) {

		// Creates an empty Shader object and returns an unsigned int by which
		// it can be referenced.  A shader object is used to maintain the 
		// source code strings that define a shader.
        GLuint shader = glCreateShader( entry->type );

		// Store the int ID for the shader in a ShaderInfo structure
        entry->shader = shader;

		// Read in the source code for a shader
        const GLchar* source = ReadShader( entry->filename );
        if ( source == NULL ) {
            for ( entry = shaders; entry->type != GL_NONE; ++entry ) {

				// Release the name (ID) that was allocated.
                glDeleteShader( entry->shader );
                entry->shader = 0;
            }

            return 0;
        }

		// Associate the shader source code with the Shader object
        glShaderSource( shader, 1, &source, NULL );

		// Release the memory holding the character array into which
		// the shader source was read
        delete [] source;
		 
		// Complie the shader source code
        glCompileShader( shader );

		// Determine if the shader compiled without errors.
		// "complied" will be set to GL_TRUE if compile operation 
		// is a success.
        GLint compiled;
        glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
#ifdef _DEBUG
            GLsizei len;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( shader, len, &len, log );
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete [] log;
#endif /* DEBUG */
            return 0;
        }
		else {
#ifdef _DEBUG
			std::cout << entry->filename << " compilation complete. " << std::endl;
#endif /* DEBUG */
		}

		// Associate the compiled shader with the shader program.
		// Shader functionality will not be available until it has be linked.
        glAttachShader( program, shader );
        
        ++entry;
    }

	glBindAttribLocation(program, 0, "vertexPosition");
	glBindAttribLocation(program, 1, "vertexNormal");
	glBindAttribLocation(program, 2, "vertexColor");
	glBindAttribLocation(program, 3, "vertexTexCoord");


#ifdef GL_VERSION_4_1
    if ( GLEW_VERSION_4_1 ) {
        // glProgramParameteri( program, GL_PROGRAM_SEPARABLE, GL_TRUE );
    }
#endif /* GL_VERSION_4_1 */
    
	// Generates a complete shader program. All required shader objects
	// must be attached prior to linking.
    glLinkProgram( program );
			
	// Determine if the shader program successfully linked.
	// "linked" will be set to GL_TRUE if link is a success.
    GLint linked;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if ( !linked ) {
#ifdef _DEBUG
        GLsizei len;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &len );

        GLchar* log = new GLchar[len+1];
        glGetProgramInfoLog( program, len, &len, log );
        std::cerr << "Shader linking failed: " << log << std::endl;
        delete [] log;
#endif /* DEBUG */

        for ( entry = shaders; entry->type != GL_NONE; ++entry ) {
            glDeleteShader( entry->shader );
            entry->shader = 0;
        }
        
        return 0;
    }
	else {

#ifdef _DEBUG
        std::cout << "Shader Program "<< program << " successfully linked";
#endif /* DEBUG */

	}
	// Check whether the program can execute given the current pipeline state.
	glValidateProgram(program);
	GLint valid;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &valid);
    if (!valid) {
#ifdef _DEBUG
		GLsizei len;
        glGetProgramiv( program, GL_VALIDATE_STATUS, &len );

        GLchar* log = new GLchar[len+1];
        glGetProgramInfoLog( program, len, &len, log );
		std::cerr << "." << std::endl << "Shader program is invalid: " << log << std::endl;
#endif /* DEBUG */
        return 0;
    }

#ifdef _DEBUG
        std::cout << " and is valid. " << std::endl;
#endif /* DEBUG */

    return program;
}

//----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cplusplus


