#pragma once
#include "Message\BaseMessageListener.h"
#include "luaT\lua_ref.h"

namespace xihad { namespace script
{
	class LuaMessageListener : public ngn::BaseMessageListener
	{
	public:
		explicit LuaMessageListener(luaT::LuaRef& obj);

		virtual void receive( ngn::GameObject& pSource, const Parameter& pEvent );

	protected:
		~LuaMessageListener();

		virtual void onDestroy() override;

	private:
		luaT::LuaRef mObject;
	};
}}

