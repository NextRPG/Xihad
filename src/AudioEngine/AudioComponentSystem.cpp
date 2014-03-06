#include "AudioComponentSystem.h"
#include "AudioEngineDefine.h"
#include "AudioComponent.h"
#include "SoundComponent.h"
#include "MusicComponent.h"

using namespace irrklang;
using namespace boost;
using namespace std;
using namespace xihad::ngn;

namespace xihad { namespace audio 
{
	struct AudioComponentSystemImpl
	{
		AudioDevice* audioDevice;
		float musicVolume;
		float soundVolume;
	};

	AudioComponentSystem::AudioComponentSystem(float musicVolume, float soundVolume, const ngn::InheritanceTree& tree) :
		BaseComponentSystem(tree), mImpl(new AudioComponentSystemImpl)
	{
#ifdef _DEBUG
		int debugState = ESEO_PRINT_DEBUG_INFO_TO_STDOUT;
#else
		int debugState = 0;
#endif // _DEBUG

		mImpl->audioDevice = createIrrKlangDevice(ESOD_AUTO_DETECT, 
			ESEO_MULTI_THREADED | ESEO_USE_3D_BUFFERS | debugState);
		
		mImpl->musicVolume = musicVolume;
		mImpl->soundVolume = soundVolume;
	}

	AudioComponentSystem::~AudioComponentSystem()
	{

	}

	void AudioComponentSystem::createNewAudioEngine()
	{
		if (mImpl->audioDevice != nullptr)
		{
			mImpl->audioDevice->stopAllSounds();
			mImpl->audioDevice->drop();
		}
#ifdef _DEBUG
		int debugState = ESEO_PRINT_DEBUG_INFO_TO_STDOUT;
#else
		int debugState = 0;
#endif

		mImpl->audioDevice = createIrrKlangDevice(ESOD_AUTO_DETECT, 
			ESEO_MULTI_THREADED | ESEO_USE_3D_BUFFERS | debugState);
	}

	void AudioComponentSystem::stopAllAudio()
	{
		if (mImpl->audioDevice != nullptr)
		{
			mImpl->audioDevice->stopAllSounds();
		}
	}

	void AudioComponentSystem::pauseAllAudio()
	{
		if (mImpl->audioDevice != nullptr)
		{
			mImpl->audioDevice->setAllSoundsPaused(true);
		}
	}

	void AudioComponentSystem::resumeAllAudio()
	{
		if (mImpl->audioDevice != nullptr)
		{
			mImpl->audioDevice->setAllSoundsPaused(false);
		}
;	}

	void AudioComponentSystem::setListenerPosition( ngn::vector3df& position, ngn::vector3df& lookdir )
	{
		if (mImpl->audioDevice != nullptr)
		{
			mImpl->audioDevice->setListenerPosition(irrklang::vec3d<float>(position.X, position.Y, position.Z), 
				irrklang::vec3d<float>(lookdir.X, lookdir.Y, lookdir.Z));
		}
	}

	void AudioComponentSystem::setGlobalMusicVolume( float volume )
	{
		if (mImpl->audioDevice != nullptr)
		{
			mImpl->musicVolume = volume;
		}
	}

	void AudioComponentSystem::setGlobalSoundVolume( float volume )
	{
		if (mImpl->audioDevice != nullptr)
		{
			mImpl->soundVolume = volume;
		}
	}
	
	Component* AudioComponentSystem::create( const string& typeName, GameObject& hostObject, const Properties& param /*= ngn::NullProperties()*/ )
	{
		Component* ret = nullptr;

		if (typeName == "Sound")
		{
			ret = new SoundComponent(typeName, hostObject, mImpl->audioDevice, mImpl->soundVolume);
		}
		else if (typeName == "Music")
		{
			ret = new MusicComponent(typeName, hostObject, mImpl->audioDevice, mImpl->musicVolume);
		}
		return ret;
	}

	void AudioComponentSystem::onStart()
	{
	}

	void AudioComponentSystem::onUpdate( const ngn::Timeline& )
	{
	}

	void AudioComponentSystem::onStop()
	{
		if (mImpl->audioDevice != nullptr)
		{
			mImpl->audioDevice->stopAllSounds();
			mImpl->audioDevice->drop();
			mImpl->audioDevice = nullptr;
		}
	}
}}
