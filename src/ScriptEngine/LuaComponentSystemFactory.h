#pragma once
#include "Engine\ComponentSystemFactory.h"
#include "Engine\irr_ptr.h"

namespace xihad { namespace ngn
{
	class GameEngine;
	class GameScene;
}}

struct lua_State;
namespace xihad { namespace script
{
	class LuaComponentSystemFactory : public ngn::ComponentSystemFactory
	{
	public:
		explicit LuaComponentSystemFactory(ngn::GameEngine* engine);

		virtual ngn::ComponentSystem* create(ngn::GameScene* scene, const std::string& systemName) ;

	protected:
		void initScriptEnv(ngn::GameScene*, lua_State*);

	private:
		ngn::GameEngine* mEngine;
	};
}}

