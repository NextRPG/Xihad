#include "AudioComponent.h"
#include <irrKlang/ik_ISoundEngine.h>
#include "AudioStopListener.h"
#include "NullSound.h"
#include "Engine/GameObject.h"
#include <iostream>

using namespace std;
using namespace xihad::ngn;
using namespace irrklang;
namespace xihad { namespace audio 
{
	AudioComponent::AudioComponent(const std::string& name, ngn::GameObject& host, ISoundEngine* audiceDevice) :
		Component(name, host), audioEngine(audiceDevice), 
		audio(&NullSound::getSingleton())
	{
		audioEngine->grab();
	}

	AudioComponent::~AudioComponent()
	{
		audioEngine->drop();
	}

	void AudioComponent::onStart()
	{
	}

	void AudioComponent::onUpdate( const ngn::Timeline& time )
	{
		// TODO Update position according to hostObject()
		// TODO Reset playback speed according to game world's time scale
	}

	void AudioComponent::onStop()
	{
		audio->stop();

		if (audio != &NullSound::getSingleton())
			setSound(0);
		else
			cerr << "Sound stop failed" << endl;
	}

	void AudioComponent::playMusic(const char* filename, E_STREAM_MODE mode)
	{
		setSound(audioEngine->play2D(filename, false, false, true, mode, true));
		audio->setSoundStopEventReceiver(this);
	}

	void AudioComponent::playSound( const char* filename, E_STREAM_MODE mode )
	{
		vector3df pos = getHostObject()->getWorldTransformMatrix().getTranslation();
		vec3df ikpos(pos.X, pos.Y, pos.Z);
		
		setSound(audioEngine->play3D(filename, ikpos, true, false, true, mode, false));
		audio->setSoundStopEventReceiver(this);
	}

	void AudioComponent::OnSoundStopped( irrklang::ISound* sound, E_STOP_EVENT_CAUSE reason, void* userData )
	{
		if (this->audio != sound)
		{
			cerr << "Attempt to stop illegal sound" << endl;
			return;
		}

		for (auto listener : listeners)
			listener->onSoundStopped(this, reason);

		setSound(0);
	}

	void AudioComponent::addAudioStopListener( AudioStopListener& listener )
	{
		listeners.insert(&listener);
	}

	void AudioComponent::removeAudioStopListener( AudioStopListener& listener )
	{
		listeners.erase(&listener);
	}

	void AudioComponent::setSound( irrklang::ISound* newSound )
	{
		assert(audio != 0);

		if (audio != newSound)
		{
			audio->drop();
			audio = newSound ? newSound : &NullSound::getSingleton();
		}
	}

//////////////////////////
/// Delegate functions ///
//////////////////////////
	void AudioComponent::setPaused( bool paused )
	{
		audio->setIsPaused(paused);
	}

	bool AudioComponent::isPaused()
	{
		return audio->getIsPaused();
	}

	void AudioComponent::stopAudio()
	{
		audio->stop();
	}

	float AudioComponent::getVolume()
	{
		return audio->getVolume();
	}

	void AudioComponent::setVolume( float volume )
	{
		audio->setVolume(volume);
	}

	void AudioComponent::setPan( float pan )
	{
		audio->setPan(pan);
	}

	float AudioComponent::getPan()
	{
		return audio->getPan();
	}

	bool AudioComponent::isLooped()
	{
		return audio->isLooped();
	}

	void AudioComponent::setLooped( bool looped )
	{
		audio->setIsLooped(looped);
	}

	bool AudioComponent::isFinished()
	{
		return audio->isFinished();
	}

	void AudioComponent::setMinDistance( float min )
	{
		audio->setMinDistance(min);
	}

	float AudioComponent::getMinDistance()
	{
		return audio->getMinDistance();
	}

	void AudioComponent::setMaxDistance( float max )
	{
		audio->setMaxDistance(max);
	}

	float AudioComponent::getMaxDistance()
	{
		return audio->getMaxDistance();
	}

	unsigned AudioComponent::getPlayPosition()
	{
		return audio->getPlayPosition();
	}

	bool AudioComponent::setPlayPosition( unsigned pos )
	{
		return audio->setPlayPosition(pos);
	}

	bool AudioComponent::setPlaybackSpeed( float speed /*= 1.0f*/ )
	{
		return audio->setPlaybackSpeed(speed);
	}

	float AudioComponent::getPlaybackSpeed()
	{
		return audio->getPlaybackSpeed();
	}

	unsigned AudioComponent::getPlayLength()
	{
		return audio->getPlayLength();
	}

}}