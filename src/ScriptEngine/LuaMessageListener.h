#pragma once
#include <Engine\Message\MessageListener.h>
#include "luaT\lua_ref.h"

namespace xihad { namespace script
{
	class LuaMessageListener : public ngn::MessageListener
	{
	public:
		explicit LuaMessageListener(luaT::LuaRef& obj);

		virtual void receive( ngn::GameObject& pSource, const Parameter& pEvent );

	protected:
		~LuaMessageListener();

	private:
		luaT::LuaRef mObject;
	};
}}

