#pragma once
#include <irrKlang/ik_ISound.h>

namespace xihad { namespace audio
{
	using namespace irrklang;

	class NullSound : public ISound
	{
	public:
		virtual ISoundSource* getSoundSource() { return nullptr; }

		virtual void setIsPaused( bool paused = true ) { }

		virtual bool getIsPaused() { return true; }

		virtual void stop() { }

		virtual ik_f32 getVolume() { return 0.f; }

		virtual void setVolume( ik_f32 volume ) { }

		virtual void setPan( ik_f32 pan ) { }

		virtual ik_f32 getPan() { return 0.f; }

		virtual bool isLooped() { return false; }

		virtual void setIsLooped( bool looped ) { }

		virtual bool isFinished() { return true; }

		virtual void setMinDistance( ik_f32 min ) { }

		virtual ik_f32 getMinDistance() { return 1.f; }

		virtual void setMaxDistance( ik_f32 max ) { }

		virtual ik_f32 getMaxDistance() { return 1000000000.f; }

		virtual void setPosition( vec3df position ) { }

		virtual vec3df getPosition() { return vec3df(); }

		virtual void setVelocity( vec3df vel ) { }

		virtual vec3df getVelocity() { return vec3df(); }

		virtual ik_u32 getPlayPosition() { return 0; }

		virtual bool setPlayPosition( ik_u32 pos ) { return false; }

		virtual bool setPlaybackSpeed( ik_f32 speed = 1.0f ) { return false; }

		virtual ik_f32 getPlaybackSpeed() { return 0.f; }

		virtual ik_u32 getPlayLength() { return 0u; }

		virtual ISoundEffectControl* getSoundEffectControl() { return 0; }

		virtual void setSoundStopEventReceiver( ISoundStopEventReceiver* reciever, void* userData=0 ) { }

		virtual void grab() { }

		virtual bool drop() { return false; }

		static NullSound& getSingleton() { return sSingleton; }

	private:	// not accessable
		NullSound() {}
		virtual ~NullSound() {}

	private:
		static NullSound sSingleton;
	};
}}

