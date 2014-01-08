#pragma once
#include "Engine/MultiComponentSystemFactory.h"

namespace xihad { namespace audio
{
	class AudioComponentSystemFactory : public ngn::MultiComponentSystemFactory
	{
	public:
		explicit AudioComponentSystemFactory();

		virtual ngn::ComponentSystem* createMainSystem( ngn::GameScene* scene );

	private:
		void init();
	};
}}

