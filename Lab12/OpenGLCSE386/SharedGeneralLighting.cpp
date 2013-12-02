#include "SharedGeneralLighting.h"

#include <sstream> 

SharedGeneralLighting::SharedGeneralLighting(GLuint bindingPoint)
	: MAX_LIGHTS(8), bindingPoint(bindingPoint), blockSizeAndOffetsSet(false)
{
}

SharedGeneralLighting::~SharedGeneralLighting(void)
{
	lights.clear();
}

void SharedGeneralLighting::findOffets( GLuint shaderProgram )
{
	// Get an identifier for a buffer
	glGenBuffers(1, &buffer);

	// Bind the buffer
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
 
	// Allocate the buffer. Does not load data. Note the use of NULL where the data would normally be.
	// Data is not loaded because the above struct will not be byte alined with the uniform block.
	glBufferData(GL_UNIFORM_BUFFER, blockSize, NULL, GL_DYNAMIC_DRAW);

	// Assign the buffer to a binding point to be the same as the uniform in the shader(s). In this case 1.
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, buffer);

	blockSizeAndOffetsSet = true;
}

void SharedGeneralLighting::findBlockSizeSetBindingPoint( GLuint shaderProgram )
{
	// Get the index of the "ColorBlock"
	blockIndex = glGetUniformBlockIndex(shaderProgram, "LightBlock"); 
	cout << "Block index is " << blockIndex << endl;

	// Determine the size in bytes of the uniform block.
	glGetActiveUniformBlockiv(shaderProgram, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	cout << "Block size is " << blockSize << endl;

	// Assign the block to a binding point. In this case 1.
	glUniformBlockBinding(shaderProgram, blockIndex, bindingPoint);

}

// returns true if all uniform locations are found
void SharedGeneralLighting::setUniformBlockForShader(GLuint shaderProgram)
{
	findBlockSizeSetBindingPoint(shaderProgram);

	if ( blockSizeAndOffetsSet == false ) {

		findOffets(shaderProgram);

		for( int i = 0; i < MAX_LIGHTS; i++ ) {

			GeneralLight light = initilizeLight( shaderProgram, i );

			lights.push_back( light );
		}

		for( int i = 0; i < MAX_LIGHTS; i++ ) {

			initilizeAttributes( i );

		}
	}
}



GeneralLight SharedGeneralLighting::initilizeLight(GLuint shaderProgram, GLint lightNumber)
{
	static const int NUM_LIGHT_ATTRIBUTES = 12;
	
	GeneralLight light;

	// Get string representation of the int
	string numberStrg = static_cast<ostringstream*>( &(ostringstream() << lightNumber) )->str();

	string uniformNames[NUM_LIGHT_ATTRIBUTES];

	GLuint uniformIndices[NUM_LIGHT_ATTRIBUTES] = {0};
	GLint uniformOffsets[NUM_LIGHT_ATTRIBUTES] = {0};

	// Get the indices of the uniform block variables in the shader
	static const string names[NUM_LIGHT_ATTRIBUTES] 
					= { "ambientColor", "diffuseColor", "specularColor",
						"positionOrDirection", "spotDirection",
						"isSpot", "spotCutoffCos", "spotExponent",
						 "constant", "linear", "quadratic", "enabled"};

	for(int i = 0; i < NUM_LIGHT_ATTRIBUTES; i++ ) {

		uniformNames[i] = "lights["+ numberStrg + "]." + names[i];
			//sprintf("lights[%s].%s", numberStrg.c_str(), names[i]);
			//(char * )(("lights["+ numberStrg + "]." + names[i]).c_str());

	}

	const GLchar * charStringNames[] = {uniformNames[0].c_str(), uniformNames[1].c_str(), 
										uniformNames[2].c_str(), uniformNames[3].c_str(), 
										uniformNames[4].c_str(), uniformNames[5].c_str(), 
										uniformNames[6].c_str(), uniformNames[7].c_str(), 
										uniformNames[8].c_str(), uniformNames[9].c_str(), 
										uniformNames[10].c_str(), uniformNames[11].c_str() }; 

	//for(int i = 0; i < NUM_LIGHT_ATTRIBUTES; i++ ) {

	//	cout << charStringNames[i] << " "; 
	//}

	cout << endl;

	glGetUniformIndices(shaderProgram, NUM_LIGHT_ATTRIBUTES, (const GLchar **)charStringNames, uniformIndices);

	for(int i = 0; i < NUM_LIGHT_ATTRIBUTES; i++ ) {

		cout << charStringNames[i] << " indice is " <<  uniformIndices[i] << endl; 
	}

	cout << endl;

	//Get the offsets of the uniforms. The offsets in the buffer will be the same.
	glGetActiveUniformsiv(shaderProgram, NUM_LIGHT_ATTRIBUTES, uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets);

	for(int i = 0; i < NUM_LIGHT_ATTRIBUTES; i++ ) {

		cout << charStringNames[i] << " offset is " <<  uniformOffsets[i] << endl; 
	}

	// Light color locations
	light.ambientColorLoc = uniformOffsets[0];
	light.diffuseColorLoc = uniformOffsets[1];
	light.specularColorLoc = uniformOffsets[2];

	// Position and direction location
	light.positionOrDirectionLoc = uniformOffsets[3];

	// Spotlight locations
	light.spotDirectionLoc = uniformOffsets[4];
	light.isSpotLoc = uniformOffsets[5];
	light.spotCutoffCosLoc = uniformOffsets[6];
	light.spotExponentLoc = uniformOffsets[7];

	//Attenuation factor locations
	light.constantLoc = uniformOffsets[8];
	light.linearLoc = uniformOffsets[9];
	light.quadraticLoc = uniformOffsets[10];

	// Enabled location
	light.enabledLoc = uniformOffsets[11];


	return light;
}

void SharedGeneralLighting::initilizeAttributes( GLint lightNumber )
{
	setEnabled( (lightSource)lightNumber, false );
	
	setAmbientColor( (lightSource)lightNumber, vec4(0.0f, 0.0f, 0.0f, 1.0f));
	setDiffuseColor( (lightSource)lightNumber, vec4(0.0f, 0.0f, 0.0f, 1.0f));
	setSpecularColor( (lightSource)lightNumber, vec4(0.0f, 0.0f, 0.0f, 1.0f));

	setPositionOrDirection( (lightSource)lightNumber, vec4(0.0f, 0.0f, -1.0f, 0.0f));
	
	setIsSpot( (lightSource)lightNumber, false );
	setSpotDirection( (lightSource)lightNumber, vec3(-1.0f, 1.0f, 0.0f) );
	setSpotCutoffCos( (lightSource)lightNumber, cos(glm::radians(180.0f)) );
	setSpotExponent( (lightSource)lightNumber, 50.0f);

	setConstantAttenuation( (lightSource)lightNumber, 1.0f );
	setLinearAttenuation( (lightSource)lightNumber, 0.0f );
	setQuadraticAttenuation( (lightSource)lightNumber, 0.0f );
}


void SharedGeneralLighting::setEnabled(lightSource light, bool on)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);

	lights[light].enabled = on;

	int intOn = 0;
	if (lights[light].enabled) {
		intOn = 1;
	}

	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].enabledLoc, sizeof(int),  &intOn);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setAmbientColor(lightSource light, vec4 color)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);

	lights[light].ambientColor = color;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].ambientColorLoc, sizeof(vec4), value_ptr(lights[light].ambientColor));

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setDiffuseColor(lightSource light, vec4 color)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);

	lights[light].diffuseColor = color;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].diffuseColorLoc, sizeof(vec4), value_ptr(lights[light].diffuseColor));

	// Unbind the buffer.
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setSpecularColor(lightSource light, vec4 color)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);

	lights[light].specularColor = color;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].specularColorLoc, sizeof(vec4), value_ptr(lights[light].specularColor));

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setPositionOrDirection(lightSource light, vec4 positOrDirect)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);

	lights[light].positionOrDirection = positOrDirect;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].positionOrDirectionLoc, sizeof(vec4), value_ptr(lights[light].positionOrDirection));

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setAttenuationFactors( lightSource light, vec3 factors)
{
	setConstantAttenuation(light, factors.x);
	setLinearAttenuation(light, factors.y);
	setQuadraticAttenuation(light, factors.z);

}

void SharedGeneralLighting::setConstantAttenuation(lightSource light, float factor)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);

	lights[light].constant = factor;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].constantLoc, sizeof(float),  &lights[light].constant);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setLinearAttenuation(lightSource light, float factor)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);

	lights[light].linear = factor;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].linearLoc, sizeof(float),  &lights[light].linear);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setQuadraticAttenuation(lightSource light, float factor)
{
	// Bind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);

	lights[light].quadratic = factor;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].quadraticLoc, sizeof(float),  &lights[light].quadratic);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setIsSpot( lightSource light, bool spotOn)
{
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);

	lights[light].isSpot = spotOn;
	int intIsSpot = 0;
	if (lights[light].enabled) {
		intIsSpot = 1;
	}

	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].isSpotLoc, sizeof(int),  &intIsSpot);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setSpotDirection( lightSource light, vec3 spotDirect)
{
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);

	lights[light].spotDirection = spotDirect;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].spotDirectionLoc, sizeof(vec3), value_ptr(lights[light].spotDirection));

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setSpotCutoffCos( lightSource light, float cutoffCos)
{
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);

	lights[light].spotCutoffCos = cutoffCos;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].spotCutoffCosLoc, sizeof(float),  &lights[light].spotCutoffCos);
	
	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SharedGeneralLighting::setSpotExponent( lightSource light, float spotEx)
{
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);

	lights[light].spotExponent = spotEx;
	glBufferSubData(GL_UNIFORM_BUFFER, lights[light].spotExponentLoc, sizeof(float),  &lights[light].spotExponent);

	// Unbind the buffer. 
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}