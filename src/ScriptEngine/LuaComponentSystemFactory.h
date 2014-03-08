#pragma once
#include "Engine\ComponentSystemFactory.h"
#include "Engine\irr_ptr.h"

namespace xihad { namespace ngn
{
	class GameEngine;
	class GameScene;
}}

namespace xihad { namespace script
{
	class LuaComponentSystemFactory : public ngn::ComponentSystemFactory
	{
	public:
		explicit LuaComponentSystemFactory(ngn::GameEngine* engine);

		virtual ngn::ComponentSystem* create(ngn::GameScene* scene, const std::string& systemName) ;

		void setScriptPath(const std::string& path)
		{
			mScriptPath = path;
		}

		const std::string& getScriptPath() const
		{
			return mScriptPath;
		}

	protected:
		void initScriptEnv(ngn::GameScene*);

	private:
		std::string mScriptPath;
		ngn::GameEngine* mEngine;
	};
}}

