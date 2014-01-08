#pragma once
#include "AudioComponent.h"

namespace xihad { namespace audio 
{
	class MusicComponent : public AudioComponent
	{
	public:
		MusicComponent(const std::string& name, ngn::GameObject& host, AudioDevice* audiceDevice, float& globalVolume);

		void playMusic(const std::string file, bool isLooped = false, int fadeMilliSeconds = 1500, SoundEffect effect = NoEffect);
		
	protected:
		virtual ~MusicComponent();

	private:

	};

}}
