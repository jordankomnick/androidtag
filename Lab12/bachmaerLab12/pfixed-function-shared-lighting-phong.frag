// Targeting version 1.5 of GLSL. If the GLSL compiler does not support 
// 1.5 or newer it will cause an error.
#version 150

#pragma optimize(off)
#pragma debug(on)

// Declare default precision - eliminates warnings (but can cause an error on some machines)
//precision mediump float;

// Structure for holding general light properties
struct GeneralLight {
  
	vec4 ambientColor;		// ambient color of the light
	vec4 diffuseColor;		// diffuse color of the light
    vec4 specularColor;		// specular color of the light

	// Either the position or direction
	// if w = 0 then the light is directional
	// if w = 1 then the light is positional
	// direction is the negative of the direction the light is shinning
    vec4 positionOrDirection;    
					  
	// spotlight attributes
    vec3 spotDirection;		// the direction the cone of light is shinning    
	int isSpot;				// 1 if the light is a spotlight  
    float spotCutoffCos;	// Cosine of the spot cutoff angle
    float spotExponent;		// For gradual falloff near cone edge

	// attenuation coefficients
    float constant; 
    float linear;
    float quadratic;

	int enabled;			// 1 if light is "on"

};

// Structure for material properties
struct Material
{
	vec4 ambientMat;

	vec4 diffuseMat;

	vec4 specularMat;

	float specularExp;

	vec4 emissiveMat;

	bool textureMapped;
};

const int MaxLights = 8;

layout (std140) uniform LightBlock
{
 GeneralLight lights[MaxLights];
};


uniform Material object;

layout (std140) uniform worldEyeBlock
{
	vec3 worldEyePosition;
};

// Vertex attributes passed in by the vertex shader
in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;  

// Output color for the fragment (pixel)
out vec4 FragColor;

vec4 calcLight( GeneralLight light, Material object )
{
	vec4 totalLight = vec4(0.0f,0.0f,0.0f,0.0f);
	
	if(light.enabled == 1) {

		float distance = -1.0f;

		// Ambient
		vec4 ambientTerm = object.ambientMat * light.ambientColor;

		// Find a unit vector that points at the light source
		vec3 directionToLight;
		if ( light.positionOrDirection.w < 1 ) {

			directionToLight = normalize( light.positionOrDirection.xyz );
		}
		else {
			
			vec3 positionToLight = light.positionOrDirection.xyz - WorldPos0;
			distance = length( positionToLight ); 
			directionToLight = normalize( positionToLight );
		}

		// Diffuse
		float DiffuseFactor = max(dot(Normal0, directionToLight), 0.0f);  
		vec4 diffuseTerm = DiffuseFactor * object.diffuseMat * light.diffuseColor;

		// Specular
		vec3 VertexToEye = normalize(worldEyePosition - WorldPos0);
		vec3 LightReflect = normalize(reflect(-directionToLight, Normal0));
		float SpecularFactor = max(dot(VertexToEye, LightReflect), 0.0f);
		SpecularFactor = pow(SpecularFactor, object.specularExp);	
		vec4 specularTerm = SpecularFactor * object.specularMat * light.specularColor;

		float spotLightEffect = 1.0f;
	
		// Calculate spotlight effect
		if( light.isSpot == 1 ) {

			vec3 LightToPixel = -directionToLight;  
			float spotCosFactor = dot(LightToPixel, light.spotDirection); 

			if( spotCosFactor > light.spotCutoffCos ) {

				//spotLightEffect =  pow( spotCosFactor, light.spotExponent);

				spotLightEffect = 1.0f - (1.0f - spotCosFactor) / (1.0f - light.spotCutoffCos);

			}
			else {
				spotLightEffect = 0.0f;
			}

		}

		// Calculate attenuation term
		float attenuation = 1.0f;

		if ( distance >  0.0f ) {

			attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

		}

		totalLight = attenuation * (ambientTerm +  (spotLightEffect  * (diffuseTerm + specularTerm)));
	}


	return totalLight;
}

uniform sampler2D gSampler;

// *** Fog ***
const vec4 fogColor = vec4(0.5f, 0.5f, 0.5f, 0.0f);
const float fogEnd = 20.0f;
const float fogStart = 1.0f;

const float fogDensity = 0.2f;

float distanceFromViewPoint;

float linearFogFactor() {

	distanceFromViewPoint = distance(worldEyePosition, WorldPos0);

	return max((fogEnd - distanceFromViewPoint)/(fogEnd - fogStart), 0.0f);

}

float exponentialFogFactor() {

	distanceFromViewPoint = distance(worldEyePosition, WorldPos0);	
	return exp( -(fogDensity * distanceFromViewPoint));
}

float exponentialTwoFogFactor() {

	distanceFromViewPoint = distance(worldEyePosition, WorldPos0);
	return exp( - pow((fogDensity * distanceFromViewPoint),2));
}

void main()
{
	
	FragColor = object.emissiveMat;

	Material texturedMaterial = object;

	float alpha = object.diffuseMat.a;

	if (object.textureMapped) {
		texturedMaterial.ambientMat = texture2D(gSampler, TexCoord0.st);
		texturedMaterial.diffuseMat = texture2D(gSampler, TexCoord0.st);
		texturedMaterial.specularMat = texture2D(gSampler, TexCoord0.st);
	}

	for (int i = 0; i < MaxLights; i++)  {
	
		FragColor += calcLight( lights[i], texturedMaterial );
	}

	// *** Fog ***
	//float fogFactor = linearFogFactor();
	//float fogFactor = exponentialFogFactor();
	//float fogFactor = exponentialTwoFogFactor();

	//FragColor = fogFactor * FragColor + (1-fogFactor) * fogColor;

	FragColor.a = alpha;

}