#include "AudioComponent.h"
#include <irrKlang/ik_ISoundEngine.h>
#include "AudioStopListener.h"
#include "NullSound.h"
#include "Engine/GameObject.h"
#include <iostream>
#include "Engine/Process.h"

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
		setSound(0);
	}

	void AudioComponent::playMusic(const char* filename, E_STREAM_MODE mode)
	{
		setSound(audioEngine->play2D(filename, true, false, true, mode, true));
	}

	void AudioComponent::playSound( const char* filename, E_STREAM_MODE mode )
	{
		vector3df pos = getHostObject()->getWorldTransformMatrix().getTranslation();
		vec3df ikpos(pos.X, pos.Y, pos.Z);
		
		setSound(audioEngine->play3D(filename, ikpos, false, false, true, mode, false));
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

		assert(!isNull(audio));
		{
			audio->setSoundStopEventReceiver(0);
			audio->drop();
			audio = &NullSound::getSingleton();
		}
		assert(isNull(audio));
		// assert(There isn't any ISound keeping this pointer as its StopEventReceiver);
	}

	void AudioComponent::setSound( irrklang::ISound* newSound )
	{
		newSound = newSound ? newSound : &NullSound::getSingleton();

		bool wasPaused = newSound->getIsPaused();

		newSound->setIsPaused(true);
		setSound_(newSound);

		assert(isNull(audio) || !newSound->isFinished());
		newSound->setIsPaused(wasPaused);
	}

	void AudioComponent::setSound_( irrklang::ISound* newSound )
	{
		assert(newSound->getIsPaused());
		assert(newSound);

		assert(audio && "Audio must be ISound or NullSound");
		{	// Stop audio and wait until audio was dropped
			audio->stop();

			while (!isNull(audio))
				Process::sleep(0);
		}
		assert(isNull(audio));

		// Process::sleep(1); // Test extreme case for dead lock 

		// Avoid to listen a finished sound
		if (!newSound->isFinished())
		{
			audio = newSound;
			audio->setSoundStopEventReceiver(this);
		}
	}

	void AudioComponent::addAudioStopListener( AudioStopListener& listener )
	{
		listeners.insert(&listener);
	}

	void AudioComponent::removeAudioStopListener( AudioStopListener& listener )
	{
		listeners.erase(&listener);
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

	bool AudioComponent::isNull( irrklang::ISound* sound )
	{
		return sound == &NullSound::getSingleton() || !sound;
	}

}}