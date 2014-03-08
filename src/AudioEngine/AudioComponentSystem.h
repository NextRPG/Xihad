#pragma once
#include "Engine\BaseComponentSystem.h"
#include <boost\scoped_ptr.hpp>
#include "AudioEngineDefine.h"

namespace irrklang
{
	class ISoundEngine;
}

namespace xihad { namespace audio 
{
	struct AudioComponentSystemImpl;

	class AudioComponentSystem : public ngn::BaseComponentSystem
	{
	public:
		AudioComponentSystem(float musicVolume, float soundVolume, const ngn::InheritanceTree& tree);
		virtual ~AudioComponentSystem();

		//全局的AudioEngine控制方法
		void createNewAudioEngine();
		void stopAllAudio();
		void pauseAllAudio();
		void resumeAllAudio();
		void setListenerPosition(ngn::vector3df& position, ngn::vector3df& lookdir);
		void setGlobalMusicVolume(float volume);
		void setGlobalSoundVolume(float volume);

		virtual ngn::Component* create( const std::string& typeName, 
			ngn::GameObject& hostObject, const ngn::Properties& param = ngn::NullProperties()) override;

		virtual void onStart();
		virtual void onUpdate( const ngn::Timeline& );
		virtual void onStop();
		//virtual void onDestroy();

	private:
		boost::scoped_ptr<AudioComponentSystemImpl> mImpl;
	};

}}