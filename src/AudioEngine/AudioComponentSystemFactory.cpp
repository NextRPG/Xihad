#include "AudioComponentSystemFactory.h"
#include "AudioComponentSystem.h"
#include <Engine/GameScene.h>
#include <ScriptEngine/LuaComponentSystem.h>

namespace xihad { namespace audio 
{
	int luaopen_AudioComponents(lua_State* L, AudioComponentSystem*);

	using namespace ngn;
	AudioComponentSystemFactory::AudioComponentSystemFactory() :
		MultiComponentSystemFactory("Audio")
	{
	}

	ComponentSystem* AudioComponentSystemFactory::createMainSystem(GameScene* scene)
	{
		AudioComponentSystem* acs = new AudioComponentSystem(*this);
		if (scene->hasSystem("Lua"))
		{
			auto lcs = static_cast<script::LuaComponentSystem*>(scene->requireSystem("Lua"));
			luaopen_AudioComponents(lcs->getLuaState(), acs);
		}

		return acs;
	}

}}

