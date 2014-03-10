#include "AudioComponentSystemFactory.h"
#include "AudioComponentSystem.h"

namespace xihad { namespace audio 
{


	AudioComponentSystemFactory::AudioComponentSystemFactory() :
		MultiComponentSystemFactory("Audio")
	{
	}

	ngn::ComponentSystem* AudioComponentSystemFactory::createMainSystem( ngn::GameScene* scene )
	{
		return new AudioComponentSystem(*this);
	}

}}

