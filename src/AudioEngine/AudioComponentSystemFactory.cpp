#include "AudioComponentSystemFactory.h"
#include "AudioComponentSystem.h"

namespace xihad { namespace audio 
{


	AudioComponentSystemFactory::AudioComponentSystemFactory() :
		MultiComponentSystemFactory("Audio")
	{
		init();
	}

	ngn::ComponentSystem* AudioComponentSystemFactory::createMainSystem( ngn::GameScene* scene )
	{
		//从配置设置里读取音量
		float soundVolume = 1.0f;
		float musicVolume = 1.0f;

		return new AudioComponentSystem(soundVolume, musicVolume, *this);
	}

	void AudioComponentSystemFactory::init()
	{
		registerDerivedType("Sound");
		registerDerivedType("Music");
	}

}}

