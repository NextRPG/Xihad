#pragma once
#include <Engine/MultiComponentSystemFactory.h>
#include <Engine/irr_ptr.h>
#include "AnimationClipsCache.h"

namespace irr
{
	class IrrlichtDevice;
	struct SIrrlichtCreationParameters;
}

namespace xihad { namespace render3d 
{
	class IrrlichtComponentSystemFactory : public ngn::MultiComponentSystemFactory
	{
	public:
		explicit IrrlichtComponentSystemFactory(irr::IrrlichtDevice* device);

		irr::IrrlichtDevice* getDevice();

		virtual ngn::ComponentSystem* createMainSystem( ngn::GameScene* scene );

	private:
		void init();

	private:
		irr_ptr<irr::IrrlichtDevice> mDevice;
		AnimationClipsCache mCachedClips;
	};
}}