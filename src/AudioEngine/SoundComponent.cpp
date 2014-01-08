#include "SoundComponent.h"

namespace xihad { namespace audio
{
	SoundComponent::SoundComponent( const std::string& name, ngn::GameObject& host, AudioDevice* audiceDevice, float& globalVolume ) :
		AudioComponent(name, host, audiceDevice, globalVolume)
	{
	}
	SoundComponent::~SoundComponent()
	{
	}
	void SoundComponent::playSound( const std::string file, ngn::vector3df& position, bool isLooped /*= false*/, float minDistance /*= 1*/, int fadeMilliSeconds /*= 0*/, SoundEffect effect /*= NoEffect*/ )
	{
		audioInstance = audioEngine->play3D(file.c_str(), irrklang::vec3d<float>(position.X, position.Y, position.Z), isLooped, true, false, irrklang::ESM_NO_STREAMING, true);
		if (audioInstance)
		{
			audioInstance->setVolume(volume);
			setEffect(audioInstance, effect);
			audioInstance->setIsPaused(false);
		}
	}

	void SoundComponent::setMinDistance( float min )
	{
		audioInstance->setMinDistance(min);
	}

	void SoundComponent::setPosition( ngn::vector3df& position )
	{
		audioInstance->setPosition(irrklang::vec3d<float>(position.X, position.Y, position.Z));
	}
}}