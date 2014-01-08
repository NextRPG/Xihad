#pragma once
#include "Engine\ComponentSystemFactory.h"
#include "Engine\irr_ptr.h"

namespace irr {
	class IrrlichtDevice;
}

namespace xihad { namespace script
{
	class LuaComponentSystemFactory : public ngn::ComponentSystemFactory
	{
	public:
		explicit LuaComponentSystemFactory(irr::IrrlichtDevice* device);

		virtual ngn::ComponentSystem* create(
			ngn::GameScene* scene, const std::string& systemName) ;

		void setScriptPath(const std::string& path)
		{
			mScriptPath = path;
		}

		const std::string& getScriptPath() const
		{
			return mScriptPath;
		}

	private:
		std::string mScriptPath;
		irr_ptr<irr::IrrlichtDevice> mDevice;
	};
}}

