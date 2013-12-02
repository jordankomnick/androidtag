#ifndef ___LIGHTING_STRUCTS_H___
#define ___LIGHTING_STRUCTS_H___

#include <iostream>

using namespace std;

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

GLint getUniformLocation(GLuint const m_shaderProg, char* pUniformName);

vec3 findUnitNormal (vec3 v0, vec3 v1, vec3 v2);

struct Attenuation {

	GLint constantLoc;
	GLint linearLoc;
	GLint quadraticLoc;
};


struct AmbientLight
{
	AmbientLight( GLint colorIntensityLoc = 0xFFFFFFFF,  GLint enabledLoc = 0xFFFFFFFF )
	{
		this->colorIntensityLoc = colorIntensityLoc;
		this->enabledLoc = enabledLoc;
	}

	vec4 getColorIntensity() { return colorIntensity; }

	bool getEnabled() {return enabled; }

	void setColorIntensity(vec4 c)
	{
		this->colorIntensity = c;
		if (colorIntensityLoc != 0xFFFFFFFF) {

			glUniform4fv(colorIntensityLoc, 1, value_ptr(colorIntensity));
		}
		else {
			cout << "Light color and intensity location not set." << endl;
		}
	}

	void setEnabled(bool e)
	{
		this->enabled = e;
		if (enabledLoc != 0xFFFFFFFF) {
			glUniform1i(enabledLoc, enabled);
		}
		else {
			cout << "Light enabled location not set." << endl;
		}
	}

	protected:

	GLint colorIntensityLoc;
	vec4 colorIntensity;

	GLint enabledLoc;
	bool enabled;
};


struct DirectionalLight : public AmbientLight
{
	DirectionalLight( GLint colorIntensityLoc = 0xFFFFFFFF,  GLint directionLoc = 0xFFFFFFFF, 
					  GLint specularIntensityLoc = 0xFFFFFFFF,  GLint  enabledLoc = 0xFFFFFFFF)
		: AmbientLight( colorIntensityLoc, enabledLoc ) 
	{
		this->specularIntensityLoc = specularIntensityLoc;
		this->directionLoc = directionLoc;
	}

	vec4 getSpecularIntensity() { return specularIntensity; }

	vec3 getDirection() {return direction; }

	void setSpecularIntensity(vec4 c)
	{
		this->specularIntensity = c;

		if (specularIntensityLoc != 0xFFFFFFFF) {

			glUniform4fv(specularIntensityLoc, 1, value_ptr(specularIntensity));
		}
		else {
			cout << "Specular color and intensity location not set." << endl;
		}
	}

	void setDirection(vec3 d)
	{
		this->direction = normalize( d );
		if (directionLoc != 0xFFFFFFFF) {
			glUniform3fv(directionLoc, 1, value_ptr(direction));
		}
		else {
			cout << "direction location not set." << endl;
		}
	}

	protected:

	GLint specularIntensityLoc;
	vec4 specularIntensity;

	GLint directionLoc;
	vec3 direction;
};


struct PositionalLight : public AmbientLight
{
	PositionalLight( GLint colorIntensityLoc = 0xFFFFFFFF,  GLint positionLoc = 0xFFFFFFFF, 
					 GLint specularIntensityLoc = 0xFFFFFFFF,  GLint  enabledLoc = 0xFFFFFFFF)
		: AmbientLight( colorIntensityLoc, enabledLoc ) 
	{
		this->specularIntensityLoc = specularIntensityLoc;
		this->positionLoc = positionLoc;
	}

	vec4 getSpecularIntensity() { return specularIntensity; }

	vec3 getPosition() {return position; }

	void setSpecularIntensity(vec4 c)
	{
		this->specularIntensity = c;

		if (specularIntensityLoc != 0xFFFFFFFF) {

			glUniform4fv(specularIntensityLoc, 1, value_ptr(specularIntensity));
		}
		else {
			cout << "Specular color and intensity location not set." << endl;
		}
	}

	void setPosition(vec3 p)
	{
		this->position =  p;
		if (positionLoc != 0xFFFFFFFF) {
			glUniform3fv(positionLoc, 1, value_ptr(position));
		}
		else {
			cout << "position location not set." << endl;
		}
	}

	protected:

	GLint specularIntensityLoc;
	vec4 specularIntensity;

	GLint positionLoc;
	vec3 position;

	GLint AttentuationLoc;

};

struct SpotLight : public PositionalLight
{
	SpotLight( GLint colorIntensityLoc = 0xFFFFFFFF,  GLint positionLoc = 0xFFFFFFFF, 
			   GLint specularIntensityLoc = 0xFFFFFFFF,  GLint spotDirectionLoc= 0xFFFFFFFF, 
			   GLint spotCutoffCosLoc = 0xFFFFFFFF, GLint  enabledLoc = 0xFFFFFFFF )
		: PositionalLight( colorIntensityLoc, positionLoc, specularIntensityLoc, enabledLoc ) 
	{
		this->spotDirectionLoc = spotDirectionLoc;
		this->spotCutoffCosLoc = spotCutoffCosLoc;
	}

	vec3 getSpotDirection() {return spotDirection; }

	float getspotCutoffCos() {return spotCutoffCos; }

	void setSpotDirection(vec3 sd)
	{
		this->spotDirection = normalize( sd );
		if (spotDirectionLoc != 0xFFFFFFFF) {
			glUniform3fv( spotDirectionLoc, 1, value_ptr(spotDirection));
		}
		else {
			cout << "spot direction location not set." << endl;
		}
	}

	void setSpotCutoffCos(float cutoff)
	{
		this->spotCutoffCos = cutoff;
		if (spotCutoffCosLoc != 0xFFFFFFFF) {
			glUniform1f(spotCutoffCosLoc, spotCutoffCos);
		}
		else {
			cout << "Spot cutoff location not set." << endl;
		}
	}

	protected:

	GLint spotDirectionLoc;
	vec3 spotDirection;

	GLint spotCutoffCosLoc;
	float spotCutoffCos;

};

struct Material
{
	Material( GLint ambientMatLoc = 0xFFFFFFFF,  GLint diffuseMatLoc = 0xFFFFFFFF, 
			  GLint specularMatLoc = 0xFFFFFFFF, GLint specularExpMatLoc= 0xFFFFFFFF, 
			  GLint emissiveMatLoc = 0xFFFFFFFF, GLint textureMappedLoc =  0xFFFFFFFF )
	{
		this->ambientMatLoc = ambientMatLoc;  
		this->diffuseMatLoc =  diffuseMatLoc;
		this->specularMatLoc = specularMatLoc;
		this->specularExpMatLoc = specularExpMatLoc;
		this->emissiveMatLoc = emissiveMatLoc;
		this->textureMappedLoc = textureMappedLoc;

		ambientMat = vec4( 0.75f, 0.75f, 0.75f, 1.0f );
		diffuseMat = vec4( 0.75f, 0.75f, 0.75f, 1.0f );
		specularMat = vec4( 1.0f, 1.0f, 1.0f, 1.0f );
		specularExpMat = 64.0f;
		emissiveMat = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
		textureMapped = false;
		
	}

	void setShaderMaterialProperties()
	{
		setAmbientMat( ambientMat );
		setDiffuseMat( diffuseMat );
		setSpecularMat( specularMat );
		setSpecularExponentMat( specularExpMat );
		setEmissiveMat( emissiveMat );
		setTextureMapped(textureMapped);
	}

	void setAmbientMat( vec4 ambientMat )
	{
		this->ambientMat = ambientMat;
		if (ambientMatLoc != 0xFFFFFFFF) {
			glUniform4fv( ambientMatLoc, 1, value_ptr(ambientMat));
		}
		else {
			cout << "ambient material location not set." << endl;
		}
	}

	void setDiffuseMat( vec4 diffuseMat )
	{
		this->diffuseMat = diffuseMat;
		if (diffuseMatLoc != 0xFFFFFFFF) {
			glUniform4fv( diffuseMatLoc, 1, value_ptr(diffuseMat));
		}
		else {
			cout << "Diffuse material location not set." << endl;
		}
	}

	void setSpecularMat( vec4 specularMat )
	{
		this->specularMat = specularMat;
		if (specularMatLoc != 0xFFFFFFFF) {
			glUniform4fv( specularMatLoc, 1, value_ptr(specularMat) );
		}
		else {
			cout << "Specular material location not set." << endl;
		}
	}

	void setSpecularExponentMat( float specularExpMat )
	{
		this->specularExpMat = specularExpMat;
		if (specularExpMatLoc != 0xFFFFFFFF) {
			glUniform1f( specularExpMatLoc, specularExpMat );

		}
		else {
			cout << "Specular exponent material location not set." << endl;
		}
	}

	void setEmissiveMat( vec4 emissiveMat )
	{
		this->emissiveMat = emissiveMat;
		if (emissiveMatLoc != 0xFFFFFFFF) {
			glUniform4fv( emissiveMatLoc, 1, value_ptr(emissiveMat) );
		}
		else {
			cout << "Emmissive material location not set." << endl;
		}
	}

	void setAmbientAndDiffuseMat( vec4 objectColor )
	{
		setAmbientMat( objectColor );
		setDiffuseMat( objectColor );

	}
	
	bool getTextureMapped() {return textureMapped; }


	void setTextureMapped(bool on)
	{
		this->textureMapped = on;
		if (textureMappedLoc != 0xFFFFFFFF) {
			glUniform1i(textureMappedLoc, textureMapped);
		}
		else {
			cout << "Texture mapped enabled location not set." << endl;
		}
	}

	protected: 

	GLint ambientMatLoc;
	vec4 ambientMat;

	GLint diffuseMatLoc;
	vec4 diffuseMat;
	
	GLint specularMatLoc;
	vec4 specularMat;
	
	GLint specularExpMatLoc;
	float specularExpMat;
	
	GLint emissiveMatLoc;
	vec4 emissiveMat;

	GLint textureMappedLoc;
	bool textureMapped;
};


#endif // ___LIGHTING_STRUCTS_H___