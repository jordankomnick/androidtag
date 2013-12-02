#include "Torus.h"

#include <vector>
#include <math.h>

#include "vertexStructs.h"

#define M_PI 3.1415926535897932384626433832795f

Torus::Torus( OpenGLApplicationBase * OpenGLApp, float innerRadius, float outerRadius, vec4 color, int sides, int rings)
	:VisualObject(OpenGLApp), objectColor(color)
{
	dInnerRadius =  innerRadius;
	dOuterRadius = outerRadius;
	nSides = sides; 
	nRings = rings;
}

Torus::~Torus(void)
{
	glDeleteVertexArrays (1, &vertexArrayObject);
}

void Torus::buildShaderProgram()
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
	assert(solidColorLocation != 0xFFFFFFFF);
}


//// Construct visual object display list.
//void Torus::initialize()
//{
//	buildShaderProgram();
//
//	double s, t, x, y, z, twopi;
//
//   twopi = 2 * (double)M_PI;
//
//   for (int i = 0; i < nRings; i++) {
//
//      glBegin(GL_QUAD_STRIP);
//      
//	  for (int j = 0; j <= nSides; j++) {
//
//		  for (int k = 1; k >= 0; k--) {
//           
//			s = (i + k) % nRings + 0.5;
//            t = j % nSides;
//
//            x = (1+.1*cos(s*twopi/nRings))*cos(t*twopi/nSides);
//            y = (1+.1*cos(s*twopi/nRings))*sin(t*twopi/nSides);
//            z = .1 * sin(s * twopi / nRings);
//
//            glVertex3f(x, y, z);
//         }
//      }
//
//      glEnd();
//   
//   }
//
//
//}


// Construct visual object display list.
void Torus::initialize()
{
	buildShaderProgram();

	//double  iradius = dInnerRadius, oradius = dOuterRadius, phi, psi, dpsi, dphi;
	//double spsi, cpsi, sphi, cphi ;

	//if ( nSides < 1 ) nSides = 1;
	//if ( nRings < 1 ) nRings = 1;

	//// Increment the number of sides and rings to allow for one more point than surface 
	//nSides ++;
	//nRings ++;

	//// vector containers to hold  data
	//vector<pnVertexData> v(nSides*nRings); // vertex positions
	//vector<unsigned int> indices; // indices
	//GLuint VBO, IBO; // Identifiers for buffer objects

	//dpsi =  2.0 * M_PI / (double)(nRings - 1) ; // increament to go through the rings
	//dphi = -2.0 * M_PI / (double)(nSides - 1) ; // increment to go through the sides
	//psi  = 0.0;

	//// Work around the donut
	//for( int j=0; j<nRings; j++ )
	//{
	//	cpsi = cos ( psi ) ;
	//	spsi = sin ( psi ) ;
	////	cout << "psi is " << psi * 180.0f / M_PI << endl;
	//	
	//	phi = 0.0;

	//	// Go all the way around the circle at one point 
	//	// on the donut
	//	for( int i=0; i<nSides; i++ )
	//	{
	//		int offset = j * nSides + i;
	//		cphi = cos ( phi ) ;
	//		sphi = sin ( phi ) ;

	//		double vx = cpsi * ( oradius + cphi * iradius ) ;
	//		double vy = spsi * ( oradius + cphi * iradius ) ;
	//		double vz = sphi * iradius;                

	//		double nx = cpsi * cphi;
	//		double ny = spsi * cphi ;
	//		double nz = sphi;

	//		v[offset] = pnVertexData(vec3(vx, vy, vz), vec3(nx, ny, nz));
 //     
	//		phi += dphi;
	//	}

	//	psi += dpsi;
	//}

	//for( int i=0; i<nSides-1; i++ )
	//{
	//	for( int j=0; j<nRings-1; j++ )
	//	{
	//		int offset = j * nSides + i;

	//		indices.push_back( offset );
	//		indices.push_back( offset + 3  );
	//		indices.push_back( offset + nSides + 3  );

	//		indices.push_back( offset );
	//		indices.push_back( offset + nSides + 3  );
	//		indices.push_back( offset + nSides  );
	//	}
	//}

	//cout << "There are " << v.size() << " vertices." << endl;
	//cout << "There are " << indices.size() << " indices." << endl;

	//glGenVertexArrays (1, &vertexArrayObject);
	//glBindVertexArray( vertexArrayObject );

	//// Create the buffer to hold interleaved data and bind the data to them
	//glGenBuffers(1, &VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO); // Buffer for vertex data
	//glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pnVertexData), &v[0], GL_STATIC_DRAW); //Buffering vertex data

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pnVertexData), 0);
	//glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pnVertexData), (const GLvoid*)sizeof(vec3) );
	//glEnableVertexAttribArray(1);

	//// Generate a buffer for the indices
	//glGenBuffers(1, &IBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0] , GL_STATIC_DRAW);

	//// store the number of indices for later use
	//indicesCount = indices.size();
 //
	//v.clear();
	//indices.clear();

	VisualObject::initialize();

} // end initialize


// Preform drawing operations
void Torus::draw()
{
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelAndFixed));
	glUniform4fv(solidColorLocation, 1, value_ptr(objectColor));

	//glBindVertexArray(vertexArrayObject);
    //glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_TRIANGLES, 96, GL_UNSIGNED_INT, (const GLvoid *)97);

	glutSolidTorus( dInnerRadius, dOuterRadius, nSides, nRings);

	// Rings are divisions of the donut. 
	// 2 rings would theoritically cut the dont in half. 4 in would make quaters
	// sides are cuts which are perpendicular to the hole. 
	// 2 sides would theoritically be like slicing a bagel so you could make a sandwich 

	// Draw all children
	VisualObject::draw();

} // end draw

