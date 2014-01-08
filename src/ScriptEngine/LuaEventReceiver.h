#pragma once
#include "Engine\UserEventReceiver.h"
#include "LuaT\lua_ref.h"

namespace xihad { namespace script
{
	class LuaEventReceiver : public ngn::UserEventReceiver
	{
	public:
		explicit LuaEventReceiver(const luaT::LuaRef& object);

		virtual bool onForegroundEvent( const ngn::KeyEvent& event );

		virtual bool onForegroundEvent( const ngn::MouseEvent& event );

		virtual bool onBackgroundEvent(const ngn::KeyEvent& event);

		virtual bool onBackgroundEvent(const ngn::MouseEvent& event);

	private:
		luaT::LuaRef lobject;
	};
}}

