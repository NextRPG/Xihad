#include "AudioComponent.h"
#include <irrKlang/ik_ISoundEngine.h>
#include <Engine/GameObject.h>
#include <Engine/Process.h>

#include "AudioStopListener.h"
#include "NullSound.h"
#include <iostream>

using namespace std;
namespace xihad { namespace audio 
{
	using namespace ngn;

	AudioComponent::AudioComponent(const string& name, GameObject& host, ISoundEngine* audiceDevice) :
		Component(name, host), audioEngine(audiceDevice), is3DAudio(false),
		audio(&NullSound::getSingleton())
	{
		audioEngine->grab();
		XIHAD_MLD_NEW_OBJECT;
	}

	AudioComponent::~AudioComponent()
	{
		audioEngine->drop();
		XIHAD_MLD_DEL_OBJECT;
	}

	void AudioComponent::onStart()
	{
		if (isPaused())
			setPaused(false);
	}

	void AudioComponent::onUpdate( const Timeline& time )
	{
		// TODO Reset playback speed according to game world's time scale
		if (is3DAudio && !isPaused())
		{
			const Matrix& wmat = getHostObject()->getWorldTransformMatrix();
			audio->setPosition(wmat.getTranslation());
		}
	}

	void AudioComponent::onStop()
	{
		setSound(0, false);
	}

	bool AudioComponent::shouldPauseWhenStart() const
	{
		return !getHostObject()->isUpdating();
	}

	void AudioComponent::play2D(const char* filename, E_STREAM_MODE mode)
	{
		bool paused = shouldPauseWhenStart();
		ISound* sound = audioEngine->play2D(filename, true, paused, true, mode);
		setSound(sound, false);
	}

	void AudioComponent::play3D( const char* filename, E_STREAM_MODE mode )
	{
		vector3df pos = getHostObject()->getWorldTransformMatrix().getTranslation();
		vec3df ikpos(pos.X, pos.Y, pos.Z);
		
		bool paused = shouldPauseWhenStart();
		ISound* sound = audioEngine->play3D(filename, ikpos, false, paused, true, mode);
		setSound(sound, true);
	}

	void AudioComponent::OnSoundStopped(ISound* sound, E_STOP_EVENT_CAUSE reason, void*)
	{
		if (this->audio != sound)
		{
			cerr << "Attempt to stop illegal sound" << endl;
			return;
		}

		for (auto listener = listeners.begin(); listener != listeners.end(); /**/)
		{
			bool continueListening = (*listener)->onAudioStopped(this, reason);

			if (!continueListening)
				listener = listeners.erase(listener);
			else 
				++listener;
		}

		assert(!isNull(audio));
		{
			audio->setSoundStopEventReceiver(0);
			audio->drop();
			changeSound(&NullSound::getSingleton(), false);
		}
		assert(isNull(audio));
		// assert(There isn't any ISound keeping this pointer as its StopEventReceiver);
	}

	void AudioComponent::setSound( ISound* newSound, bool is3d )
	{
		if (this->audio == newSound)
			return;

		newSound = newSound ? newSound : &NullSound::getSingleton();

		bool wasPaused = newSound->getIsPaused();

		newSound->setIsPaused(true);
		setSound_(newSound, is3d);

		assert(isNull(audio) || !newSound->isFinished());
		newSound->setIsPaused(wasPaused);
	}

	void AudioComponent::setSound_( ISound* newSound, bool is3d )
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
		assert(!is3DAudio);
		// Process::sleep(1); // Test extreme case for concurrent issues

		// Avoid to listen a finished sound
		if (!newSound->isFinished())
		{
			changeSound(newSound, is3d);
			audio->setSoundStopEventReceiver(this);
		}
	}

	void AudioComponent::addAudioStopListener( AudioStopListener& listener )
	{
		listeners.push_back(&listener);
	}

	void AudioComponent::removeAudioStopListener( AudioStopListener& listener )
	{
		listeners.remove(&listener);
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

	bool AudioComponent::isNull( ISound* sound )
	{
		return sound == &NullSound::getSingleton() || !sound;
	}

}}