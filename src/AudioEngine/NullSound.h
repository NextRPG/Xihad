#pragma once
#include <irrKlang/ik_ISound.h>

namespace xihad { namespace audio
{
	class NullSound : public irrklang::ISound
	{
	public:
		virtual irrklang::ISoundSource* getSoundSource() { return nullptr; }

		virtual void setIsPaused( bool paused = true ) { }

		virtual bool getIsPaused() { return true; }

		virtual void stop() { }

		virtual irrklang::ik_f32 getVolume() { return 0.f; }

		virtual void setVolume( irrklang::ik_f32 volume ) { }

		virtual void setPan( irrklang::ik_f32 pan ) { }

		virtual irrklang::ik_f32 getPan() { return 0.f; }

		virtual bool isLooped() { return false; }

		virtual void setIsLooped( bool looped ) { }

		virtual bool isFinished() { return true; }

		virtual void setMinDistance( irrklang::ik_f32 min ) { }

		virtual irrklang::ik_f32 getMinDistance() { return 1.f; }

		virtual void setMaxDistance( irrklang::ik_f32 max ) { }

		virtual irrklang::ik_f32 getMaxDistance() { return 1000000000.f; }

		virtual void setPosition( irrklang::vec3df position ) { }

		virtual irrklang::vec3df getPosition() { return irrklang::vec3df(); }

		virtual void setVelocity( irrklang::vec3df vel ) { }

		virtual irrklang::vec3df getVelocity() { return irrklang::vec3df(); }

		virtual irrklang::ik_u32 getPlayPosition() { return 0; }

		virtual bool setPlayPosition( irrklang::ik_u32 pos ) { return false; }

		virtual bool setPlaybackSpeed( irrklang::ik_f32 speed = 1.0f ) { return false; }

		virtual irrklang::ik_f32 getPlaybackSpeed() { return 0.f; }

		virtual irrklang::ik_u32 getPlayLength() { return 0u; }

		virtual irrklang::ISoundEffectControl* getSoundEffectControl() { return 0; }

		virtual void setSoundStopEventReceiver( irrklang::ISoundStopEventReceiver* reciever, void* userData=0 ) { }

		virtual void grab() { }

		virtual bool drop() { return false; }

		static NullSound& getSingleton() { return sSingleton; }

	private:	// not accessable
		NullSound();
		virtual ~NullSound();

	private:
		static NullSound sSingleton;
	};
}}

