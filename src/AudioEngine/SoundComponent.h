#pragma once
#include "AudioComponent.h"

namespace xihad { namespace audio 
{
	class SoundComponent : public AudioComponent
	{
	public:
		SoundComponent(const std::string& name, ngn::GameObject& host, AudioDevice* audiceDevice, float& globalVolume);

		void playSound(const std::string file, ngn::vector3df& position, bool isLooped = false, float minDistance = 1, int fadeMilliSeconds = 0, SoundEffect effect = NoEffect);
		void setMinDistance(float min);
		void setPosition(ngn::vector3df& position);

	protected:
		virtual ~SoundComponent();

	private:

	};

}}
