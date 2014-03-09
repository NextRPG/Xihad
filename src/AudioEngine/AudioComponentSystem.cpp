#include "AudioComponentSystem.h"
#include <irrKlang/irrKlang.h>
#include "AudioComponent.h"
#include "VectorConverter.h"

using namespace irrklang;
using namespace boost;
using namespace std;
using namespace xihad::ngn;

namespace xihad { namespace audio 
{
	struct AudioComponentSystem::impl
	{
		ISoundEngine* audioDevice;
		float playbackScale;
	};

	AudioComponentSystem::AudioComponentSystem(const ngn::InheritanceTree& tree) :
		BaseComponentSystem(tree), mImpl(new impl)
	{
#ifdef _DEBUG
		int debugState = ESEO_PRINT_DEBUG_INFO_TO_STDOUT;
#else
		int debugState = 0;
#endif // _DEBUG

		mImpl->audioDevice = createIrrKlangDevice(ESOD_AUTO_DETECT, 
			ESEO_MULTI_THREADED | ESEO_USE_3D_BUFFERS | debugState);
	}

	AudioComponentSystem::~AudioComponentSystem()
	{
		mImpl->audioDevice->drop();
		mImpl->audioDevice = nullptr;
	}

	void AudioComponentSystem::stopAllAudios()
	{
		mImpl->audioDevice->stopAllSounds();
	}

	void AudioComponentSystem::setAllAudiosPaused( bool pause )
	{
		mImpl->audioDevice->setAllSoundsPaused(false);
	}

	void AudioComponentSystem::setListenerPosition(const ngn::vector3df& position, 
		const ngn::vector3df& lookdir, const ngn::vector3df& upVec)
	{
		mImpl->audioDevice->setListenerPosition(
			to_irrklang_vector3d(position), 
			to_irrklang_vector3d(lookdir), 
			vec3df(), 
			to_irrklang_vector3d(upVec));
	}

	Component* AudioComponentSystem::create( const string& typeName, GameObject& hostObject, const Properties& param /*= ngn::NullProperties()*/ )
	{
		if (typeName == "Audio")
			return new AudioComponent(typeName, hostObject, mImpl->audioDevice);
		else 
			return 0;
	}

	void AudioComponentSystem::onStart()
	{
	}

	void AudioComponentSystem::onUpdate( const ngn::Timeline& )
	{
		// TODO
		// 更新听者信息，根据世界的 TimeScale 设置所有声音的播放速度
		// if (mImpl->playbackScale != )
		mImpl->audioDevice->update();
	}

	void AudioComponentSystem::onStop()
	{
		if (mImpl->audioDevice != nullptr)
			mImpl->audioDevice->stopAllSounds();
	}

	void AudioComponentSystem::setSoundVolume( float volume )
	{
		mImpl->audioDevice->setSoundVolume(volume);
	}

	float AudioComponentSystem::getSoundVolume()
	{
		return mImpl->audioDevice->getSoundVolume();
	}

	void AudioComponentSystem::setDefault3DSoundMinDistance( float minDistance )
	{
		mImpl->audioDevice->setDefault3DSoundMinDistance(minDistance);
	}

	float AudioComponentSystem::getDefault3DSoundMinDistance()
	{
		return mImpl->audioDevice->getDefault3DSoundMinDistance();
	}

	void AudioComponentSystem::setDefault3DSoundMaxDistance( float maxDistance )
	{
		mImpl->audioDevice->setDefault3DSoundMaxDistance(maxDistance);
	}

	float AudioComponentSystem::getDefault3DSoundMaxDistance()
	{
		return mImpl->audioDevice->getDefault3DSoundMaxDistance();
	}

}}
