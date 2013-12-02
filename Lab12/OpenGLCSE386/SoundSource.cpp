#include "SoundSource.h"

bool SoundSource::soundInitialized = false;

GLboolean OpenALInit()
{
	// Initialize Framework
	ALFWInit();
	if (!ALFWInitOpenAL())
	{
		ALFWprintf("Failed to initialize OpenAL\n");
		ALFWShutdown();
		return false;
	}
	else {
		return true;
	}
}

SoundSource::SoundSource(string soundFileName, OpenGLApplicationBase * OpenGLApp )
	:VisualObject( OpenGLApp ), soundFileName(soundFileName)
{
	if ( SoundSource::soundInitialized == false ) {

		OpenALInit();
		SoundSource::soundInitialized = true;
	}

}


SoundSource::~SoundSource(void)
{
}


void SoundSource::initialize()
{
	ALuint      uiBuffer;

	// Generate an AL Buffer
	alGenBuffers( 1, &uiBuffer );

	// Load Wave file into OpenAL Buffer
	if (!ALFWLoadWaveToBuffer(soundFileName.c_str(), uiBuffer)) {

		ALFWprintf("Failed to load %s.\n", soundFileName.c_str());
	}
	else {

		//ALFWprintf("Loaded %s sound file.\n", soundFileName.c_str());
	}

	// Generate a Source to playback the Buffer
    alGenSources( 1, &uiSource );

	// Attach Source to Buffer
	alSourcei( uiSource, AL_BUFFER, uiBuffer );
		
	// Set source properties
	alSourcef(uiSource, AL_PITCH, 1.0f); // pitch multiplier 
	alSourcef(uiSource, AL_GAIN, 1.0f); // source gain value 
	
	// determines if the positions are relative to the listener
	alSourcei(uiSource, AL_SOURCE_RELATIVE, AL_FALSE);
	
	// turns looping on (AL_TRUE) or off (AL_FALSE)	
	alSourcei(uiSource, AL_LOOPING, AL_TRUE); 

	// Position for sound sources
	alSource3f( uiSource, AL_POSITION, 0.0, 0.0, 0.0);

	// Velocity for sound sources
	alSource3f(uiSource, AL_VELOCITY, 0.0, 0.0, 0.0);

	//alSourcePlay( uiSource);
	//play();
	VisualObject::initialize();

} // end initialize



// Update this object and all of its children
// Update this object and all of its children
void SoundSource::update(float elapsedTimeSeconds)
{
	vec3 pos = getWorldPosition();

	static vec3 lastPosition = pos;

	// Position for sound sources
	alSource3f( uiSource, AL_POSITION, pos.x, pos.y, pos.z);

	if ( elapsedTimeSeconds > 0.0f) {
		vec3 velocity  = (pos - lastPosition)/elapsedTimeSeconds;

		alSource3f(uiSource, AL_VELOCITY, velocity.x, velocity.y, velocity.z );

	}

	lastPosition = pos;

	VisualObject::update(elapsedTimeSeconds);

} // end update

void SoundSource::play()
{
	// Play the source
	alSourcePlay( uiSource);
}

void SoundSource::stop()
{
	// Play the source
	alSourceStop( uiSource);
}

void SoundSource::setLooping(bool loopingOn)
{
	// turns looping on (AL_TRUE) or off (AL_FALSE)	
	alSourcei(uiSource, AL_LOOPING, loopingOn); 

}