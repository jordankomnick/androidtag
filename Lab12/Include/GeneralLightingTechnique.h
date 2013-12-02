#ifndef GENERAL_LIGHTING_TECHNIQUE_H
#define	GENERAL_LIGHTING_TECHNIQUE_H

#include <iostream>

using namespace std;

#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>

#define GLM_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/swizzle.hpp>

using namespace glm;

#include "Lighting.h"

enum lightSource {GL_LIGHT_ZERO, GL_LIGHT_ONE, GL_LIGHT_TWO, GL_LIGHT_THREE, GL_LIGHT_FOUR, GL_LIGHT_FIVE, GL_LIGHT_SIX, GL_LIGHT_SEVEN }; 

struct GeneralLight {
    
	GLint enabledLoc; bool enabled;			// true if light is "on"

	GLint ambientColorLoc; vec4 ambientColor;		// ambient color of the light
	GLint diffuseColorLoc; vec4 diffuseColor;		// diffuse color of the light
    GLint specularColorLoc; vec4 specularColor;		// specular color of the light

	// Either the position or direction
	// if w = 0 then the light is directional
	// if w = 1 then the light is positional
	// direction is the negative of the direction the light is shinning
    GLint positionOrDirectionLoc; vec4 positionOrDirection;    
					  
	// spotlight attributes
	GLint isSpotLoc; bool isSpot;			// true if the light is a spotlight
    GLint spotDirectionLoc; vec3 spotDirection;		// the direction the cone of light is shinning      
    GLint spotCutoffCosLoc; float spotCutoffCos;	// Cosine of the spot cutoff angle
    GLint spotExponentLoc; float spotExponent;		// 

	// attenuation coefficients
    GLint constantLoc; float constant; 
    GLint linearLoc; float linear;
    GLint quadraticLoc; float quadratic;
};

class GeneralLightingTechnique
{
public:
	GeneralLightingTechnique(void);
	~GeneralLightingTechnique(void);

	void initilize(GLuint shader);

	bool getEnabled(lightSource light) {return lights[light].enabled; }
	void setEnabled( lightSource light, bool on);

	vec4 getAmbientColor(lightSource light) { return lights[light].ambientColor; }
	void setAmbientColor(lightSource light, vec4 color);

	vec4 getDiffuseColor(lightSource light) { return lights[light].diffuseColor; }
	void setDiffuseColor(lightSource light, vec4 color);

	vec4 getSpecularColor(lightSource light) { return lights[light].specularColor; }
	void setSpecularColor(lightSource light, vec4 color);

	vec4 getPositionOrDirection(lightSource light) { return lights[light].positionOrDirection; }
	void setPositionOrDirection(lightSource light, vec4 positOrDirect);

	void setAttenuationFactors( lightSource light, vec3 factors);

	float getConstantAttenuation(lightSource light) { return lights[light].constant; }
	void setConstantAttenuation(lightSource light, float factor);

	float getLinearAttenuation(lightSource light) { return lights[light].linear; }
	void setLinearAttenuation(lightSource light, float factor);

	float getQuadraticAttenuation(lightSource light) { return lights[light].quadratic; }
	void GeneralLightingTechnique::setQuadraticAttenuation(lightSource light, float factor);

	bool getIsSpot( lightSource light ) {return lights[light].isSpot; }
	void setIsSpot( lightSource light, bool spotOn);

	vec3 getSpotDirection(lightSource light) { return lights[light].spotDirection; }
	void setSpotDirection(lightSource light, vec3 spotDirect);

	float getSpotCutoffCos( lightSource light ) {return lights[light].spotCutoffCos; }
	void setSpotCutoffCos( lightSource light, float cutoffCos);

	float getSpotExponent( lightSource light ) {return lights[light].spotExponent; }
	void setSpotExponent( lightSource light, float spotEx);

protected:

	GeneralLight initilizeLight(GLint lightNumber);

	void initilizeAttributes( GLint lightNumber );

	GLuint shaderProgram; 

	const GLint MAX_LIGHTS;

	vector<GeneralLight> lights;
};

#endif // GENERAL_LIGHTING_TECHNIQUE_H

