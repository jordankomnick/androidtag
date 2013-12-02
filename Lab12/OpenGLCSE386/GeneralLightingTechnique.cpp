#include "GeneralLightingTechnique.h"

#include <sstream> 

GeneralLightingTechnique::GeneralLightingTechnique(void)
	: MAX_LIGHTS(8)
{
}

GeneralLightingTechnique::~GeneralLightingTechnique(void)
{
	lights.clear();
}

// returns true if all uniform locations are found
void GeneralLightingTechnique::initilize(GLuint shader)
{
	shaderProgram = shader;

	for( int i = 0; i < MAX_LIGHTS; i++ ) {

		GeneralLight light = initilizeLight( i );

		lights.push_back( light );
	}

	for( int i = 0; i < MAX_LIGHTS; i++ ) {

		initilizeAttributes( i );

	}
}



GeneralLight GeneralLightingTechnique::initilizeLight(GLint lightNumber)
{
	GeneralLight light;

	// Get string representation of the int
	string numberStrg = static_cast<ostringstream*>( &(ostringstream() << lightNumber) )->str();

	// Enabled location
	string locationName = "lights[" + numberStrg + "].enabled"; 
	light.enabledLoc = getUniformLocation( shaderProgram, (char*)locationName.c_str());

	// Light color locations
	locationName = "lights[" + numberStrg + "].ambientColor"; 
	light.ambientColorLoc = getUniformLocation( shaderProgram, (char*)locationName.c_str());
	locationName = "lights[" + numberStrg + "].diffuseColor"; 
	light.diffuseColorLoc = getUniformLocation( shaderProgram, (char*)locationName.c_str());
	locationName = "lights[" + numberStrg + "].specularColor";
	light.specularColorLoc = getUniformLocation( shaderProgram, (char*)locationName.c_str());

	// Position and direction location
	locationName = "lights[" + numberStrg + "].positionOrDirection"; 
	light.positionOrDirectionLoc = getUniformLocation( shaderProgram, (char*)locationName.c_str());

	// Spotlight locations
	locationName = "lights[" + numberStrg + "].isSpot"; 
	light.isSpotLoc = getUniformLocation( shaderProgram, (char*)locationName.c_str());
	locationName = "lights[" + numberStrg + "].spotDirection";
	light.spotDirectionLoc = getUniformLocation( shaderProgram, (char*)locationName.c_str());
	locationName = "lights[" + numberStrg + "].spotCutoffCos"; 
	light.spotCutoffCosLoc = getUniformLocation( shaderProgram, (char*)locationName.c_str());
	locationName = "lights[" + numberStrg + "].spotExponent";
	light.spotExponentLoc = getUniformLocation( shaderProgram, (char*)locationName.c_str());

	//Attenuation factor locations
	locationName = "lights[" + numberStrg + "].constant";
	light.constantLoc = getUniformLocation( shaderProgram, (char*)locationName.c_str());
	locationName = "lights[" + numberStrg + "].linear"; 
	light.linearLoc = getUniformLocation( shaderProgram, (char*)locationName.c_str());
	locationName = "lights[" + numberStrg + "].quadratic";
	light.quadraticLoc = getUniformLocation( shaderProgram, (char*)locationName.c_str());

	return light;
}

void GeneralLightingTechnique::initilizeAttributes( GLint lightNumber )
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


void GeneralLightingTechnique::setEnabled(lightSource light, bool on)
{
	lights[light].enabled = on;
	glUniform1i(lights[light].enabledLoc, on);
}

void GeneralLightingTechnique::setAmbientColor(lightSource light, vec4 color)
{
	lights[light].ambientColor = color;
	glUniform4fv(lights[light].ambientColorLoc, 1, value_ptr(color));
}

void GeneralLightingTechnique::setDiffuseColor(lightSource light, vec4 color)
{
	lights[light].diffuseColor = color;
	glUniform4fv(lights[light].diffuseColorLoc, 1, value_ptr(color));
}

void GeneralLightingTechnique::setSpecularColor(lightSource light, vec4 color)
{
	lights[light].specularColor = color;
	glUniform4fv(lights[light].specularColorLoc, 1, value_ptr(color));
}

void GeneralLightingTechnique::setPositionOrDirection(lightSource light, vec4 positOrDirect)
{
	lights[light].positionOrDirection = positOrDirect;
	glUniform4fv(lights[light].positionOrDirectionLoc, 1, value_ptr(positOrDirect));
}

void GeneralLightingTechnique::setAttenuationFactors( lightSource light, vec3 factors)
{
	setConstantAttenuation(light, factors.x);
	setLinearAttenuation(light, factors.y);
	setQuadraticAttenuation(light, factors.z);

}

void GeneralLightingTechnique::setConstantAttenuation(lightSource light, float factor)
{
	lights[light].constant = factor;
	glUniform1f(lights[light].constantLoc, factor);
}

void GeneralLightingTechnique::setLinearAttenuation(lightSource light, float factor)
{
	lights[light].linear = factor;
	glUniform1f(lights[light].linearLoc, factor);
}

void GeneralLightingTechnique::setQuadraticAttenuation(lightSource light, float factor)
{
	lights[light].quadratic = factor;
	glUniform1f(lights[light].quadraticLoc, factor);
}

void GeneralLightingTechnique::setIsSpot( lightSource light, bool spotOn)
{
	lights[light].isSpot = spotOn;
	glUniform1i(lights[light].isSpotLoc, spotOn);
}

void GeneralLightingTechnique::setSpotDirection( lightSource light, vec3 spotDirect)
{
	lights[light].spotDirection = spotDirect;
	glUniform3fv(lights[light].spotDirectionLoc, 1, value_ptr(normalize(spotDirect)));
}

void GeneralLightingTechnique::setSpotCutoffCos( lightSource light, float cutoffCos)
{
	lights[light].spotCutoffCos = cutoffCos;
	glUniform1f(lights[light].spotCutoffCosLoc, cutoffCos);
}

void GeneralLightingTechnique::setSpotExponent( lightSource light, float spotEx)
{
	lights[light].spotExponent = spotEx;
	glUniform1f(lights[light].spotExponentLoc, spotEx);
}