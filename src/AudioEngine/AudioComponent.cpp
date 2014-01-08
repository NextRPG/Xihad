#include "AudioComponent.h"

using namespace std;
using namespace xihad::ngn;

namespace xihad { namespace audio 
{
	AudioComponent::AudioComponent( const std::string& name, ngn::GameObject& host, AudioDevice* audiceDevice, float& globalVolume ) :
		Component(name, host), audioEngine(audiceDevice), volume(globalVolume)
	{

	}
	AudioComponent::~AudioComponent()
	{
		if (audioInstance != nullptr)
		{
			audioInstance->drop();
			audioInstance = nullptr;
		}
	}

	void AudioComponent::pauseAudio()
	{
		if (audioInstance != nullptr)
		{
			audioInstance->setIsPaused(true);
		}
	}

	void AudioComponent::resumeAudio()
	{
		if (audioInstance != nullptr)
		{
			audioInstance->setIsPaused(false);
		}
	}

	void AudioComponent::stopAudio()
	{
		if (audioInstance != nullptr)
		{
			audioInstance->stop();
		}
	}

	void AudioComponent::setIsLooped( bool isLooped )
	{
		if (audioInstance != nullptr)
		{
			audioInstance->setIsLooped(isLooped);
		}
	}

	void AudioComponent::setSoundEffect( SoundEffect effect )
	{
		setEffect(audioInstance, effect);
	}

	void AudioComponent::setVolume( float volume )
	{
		if (audioInstance != nullptr)
		{
			audioInstance->setVolume(volume);
		}
	}

	void AudioComponent::setEffect( AudioInstance* instance, SoundEffect effect )
	{
		if (instance != nullptr)
		{
			switch (effect)
			{
			case xihad::audio::Chorus:
				instance->getSoundEffectControl()->enableChorusSoundEffect();
				break;
			case xihad::audio::Compressor:
				instance->getSoundEffectControl()->enableCompressorSoundEffect();
				break;
			case xihad::audio::Distortion:
				instance->getSoundEffectControl()->enableDistortionSoundEffect();
				break;
			case xihad::audio::Echo:
				instance->getSoundEffectControl()->enableEchoSoundEffect();
				break;
			case xihad::audio::Flanger:
				instance->getSoundEffectControl()->enableFlangerSoundEffect();
				break;
			case xihad::audio::Gargle:
				instance->getSoundEffectControl()->enableGargleSoundEffect();
				break;
			case xihad::audio::I3DL2Reverb:
				instance->getSoundEffectControl()->enableI3DL2ReverbSoundEffect();
				break;
			case xihad::audio::ParamEq:
				instance->getSoundEffectControl()->enableParamEqSoundEffect();
				break;
			case xihad::audio::WavesReverb:
				instance->getSoundEffectControl()->enableWavesReverbSoundEffect();
				break;
			default:
				break;
			}
		}
	}

	void AudioComponent::onStart()
	{
	}

	void AudioComponent::onUpdate( const ngn::Timeline& )
	{
	}

	void AudioComponent::onStop()
	{
		if (audioInstance != nullptr)
		{
			audioInstance->stop();
			audioInstance->drop();
			audioInstance = nullptr;
		}
	}

}}