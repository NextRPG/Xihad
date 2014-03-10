#pragma once
#include <irrKlang/ik_ISoundStopEventReceiver.h>
#include <irrKlang/ik_EStreamModes.h>
#include <set>
#include "Engine/Component.h"
#include "Engine/vector3d.h"
#include "Engine/xptr.h"

namespace irrklang 
{
	class ISound;
	class ISoundEngine;
	enum E_STOP_EVENT_CAUSE;
}

namespace xihad { namespace audio 
{
	enum SoundEffect
	{
		NoEffect, Chorus, Compressor, Distortion, 
		Echo, Flanger, Gargle, I3DL2Reverb, ParamEq, 
		WavesReverb
	};

	class AudioStopListener;
	class AudioComponent : public ngn::Component, private irrklang::ISoundStopEventReceiver
	{
	public:
		DEFINE_VISITABLE;

		AudioComponent(const std::string& name, ngn::GameObject& host, irrklang::ISoundEngine* audiceDevice);

		//! returns if the sound is paused
		virtual void setPaused( bool paused = true);

		//! returns if the sound is paused
		virtual bool isPaused();

		//! Will stop the sound and free its resources.
		/** If you just want to pause the sound, use setIsPaused().
		After calling stop(), isFinished() will usually return true. 
		Be sure to also call ->drop() once you are done.*/
		virtual void stopAudio();

		//! returns volume of the sound, a value between 0 (mute) and 1 (full volume).
		/** (this volume gets multiplied with the master volume of the sound engine
		and other parameters like distance to listener when played as 3d sound)  */
		virtual float getVolume();

		//! sets the volume of the sound, a value between 0 (mute) and 1 (full volume).
		/** This volume gets multiplied with the master volume of the sound engine
		and other parameters like distance to listener when played as 3d sound.  */
		virtual void setVolume(float volume);

		//! sets the pan of the sound. Takes a value between -1 and 1, 0 is center.
		virtual void setPan(float pan);

		//! returns the pan of the sound. Takes a value between -1 and 1, 0 is center.
		virtual float getPan();

		//! returns if the sound has been started to play looped
		virtual bool isLooped();

		//! changes the loop mode of the sound. 
		/** If the sound is playing looped and it is changed to not-looped, then it 
		will stop playing after the loop has finished. 
		If it is not looped and changed to looped, the sound will start repeating to be 
		played when it reaches its end. 
		Invoking this method will not have an effect when the sound already has stopped. */
		virtual void setLooped(bool looped);

		//! returns if the sound has finished playing.
		/** Don't mix this up with isPaused(). isFinished() returns if the sound has been
		finished playing. If it has, is maybe already have been removed from the playing list of the
		sound engine and calls to any other of the methods of ISound will not have any result.
		If you call stop() to a playing sound will result that this function will return true
		when invoked. */
		virtual bool isFinished();

		//! Sets the minimal distance if this is a 3D sound.
		/** Changes the distance at which the 3D sound stops getting louder. This works
		like this: As a listener approaches a 3D sound source, the sound gets louder.
		Past a certain point, it is not reasonable for the volume to continue to increase.
		Either the maximum (zero) has been reached, or the nature of the sound source
		imposes a logical limit. This is the minimum distance for the sound source.
		Similarly, the maximum distance for a sound source is the distance beyond
		which the sound does not get any quieter.
		The default minimum distance is 1, the default max distance is a huge number like 1000000000.0f. */
		virtual void setMinDistance(float min);

		//! Returns the minimal distance if this is a 3D sound.
		/** See setMinDistance() for details. */
		virtual float getMinDistance();

		//! Sets the maximal distance if this is a 3D sound.
		/** Changing this value is usually not necessary. Use setMinDistance() instead.
		Don't change this value if you don't know what you are doing: This value causes the sound
		to stop attenuating after it reaches the max distance. Most people think that this sets the
		volume of the sound to 0 after this distance, but this is not true. Only change the
		minimal distance (using for example setMinDistance()) to influence this.
		The maximum distance for a sound source is the distance beyond which the sound does not get any quieter.
		The default minimum distance is 1, the default max distance is a huge number like 1000000000.0f. */
		virtual void setMaxDistance(float max);

		//! Returns the maximal distance if this is a 3D sound.
		/** See setMaxDistance() for details. */
		virtual float getMaxDistance();

		//! returns the current play position of the sound in milliseconds.
		/** \return Returns -1 if not implemented or possible for this sound for example
		because it already has been stopped and freed internally or similar. */
		virtual unsigned getPlayPosition();

		//! sets the current play position of the sound in milliseconds.
        /** \param pos Position in milliseconds. Must be between 0 and the value returned
		by getPlayPosition().
		\return Returns true successful. False is returned for example if the sound already finished
		playing and is stopped or the audio source is not seekable, for example if it 
		is an internet stream or a a file format not supporting seeking (a .MOD file for example).
		A file can be tested if it can bee seeking using ISoundSource::getIsSeekingSupported(). */
		virtual bool setPlayPosition(unsigned pos);

		//! Sets the playback speed (frequency) of the sound.
		/** Plays the sound at a higher or lower speed, increasing or decreasing its
		frequency which makes it sound lower or higher.
		Note that this feature is not available on all sound output drivers (it is on the
		DirectSound drivers at least), and it does not work together with the 
		'enableSoundEffects' parameter of ISoundEngine::play2D and ISoundEngine::play3D when
		using DirectSound.
		\param speed Factor of the speed increase or decrease. 2 is twice as fast, 
		0.5 is only half as fast. The default is 1.0.
		\return Returns true if sucessful, false if not. The current sound driver might not
		support changing the playBack speed, or the sound was started with the 
		'enableSoundEffects' parameter. */
		virtual bool setPlaybackSpeed(float speed = 1.0f);

		//! Returns the playback speed set by setPlaybackSpeed(). Default: 1.0f.
		/** See setPlaybackSpeed() for details */
		virtual float getPlaybackSpeed();

		//! returns the play length of the sound in milliseconds.
		/** Returns -1 if not known for this sound for example because its decoder
		does not support length reporting or it is a file stream of unknown size.
		Note: You can also use ISoundSource::getPlayLength() to get the length of 
		a sound without actually needing to play it. */
		virtual unsigned getPlayLength();

		virtual void playMusic(const char* musicFilename, 
			irrklang::E_STREAM_MODE mode = irrklang::ESM_AUTO_DETECT);
		
		virtual void playSound(const char* soundFileName, 
			irrklang::E_STREAM_MODE mode = irrklang::ESM_AUTO_DETECT);

		virtual void addAudioStopListener(AudioStopListener&);

		virtual void removeAudioStopListener(AudioStopListener&);

	protected:
		virtual ~AudioComponent();
		
		virtual void onStart();
		virtual void onUpdate( const ngn::Timeline& );
		virtual void onStop();

	private:
		virtual void OnSoundStopped(irrklang::ISound* sound, 
				irrklang::E_STOP_EVENT_CAUSE reason, void* userData);

		void setSound(irrklang::ISound* newSound);

		/// newSound != 0 && newSound is paused
		void setSound_(irrklang::ISound* newSound);

		bool isNull(irrklang::ISound*);

	private:
		irrklang::ISoundEngine* audioEngine;
		irrklang::ISound* audio;
		std::set<xptr<AudioStopListener> > listeners;
	};

}}
