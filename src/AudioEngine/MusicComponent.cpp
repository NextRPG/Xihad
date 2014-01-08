#include "MusicComponent.h"

namespace xihad { namespace audio
{
	MusicComponent::MusicComponent( const std::string& name, ngn::GameObject& host, AudioDevice* audiceDevice, float& globalVolume ) :
		AudioComponent(name, host, audiceDevice, globalVolume)
	{
	}
	MusicComponent::~MusicComponent()
	{
	}

	void MusicComponent::playMusic( const std::string file, bool isLooped /*= false*/, int fadeMilliSeconds /*= 1500*/, SoundEffect effect /*= NoEffect*/ )
	{
		audioInstance = audioEngine->play2D(file.c_str(), isLooped, true, false, irrklang::ESM_STREAMING, true);
		if (audioInstance)
		{
			audioInstance->setVolume(volume);
			setEffect(audioInstance, effect);
			audioInstance->setIsPaused(false);
		}
	}

}}
