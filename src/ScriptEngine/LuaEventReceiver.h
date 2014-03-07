#pragma once
#include "Engine\UserEventReceiver.h"
#include "LuaT\lua_ref.h"

namespace xihad { namespace script
{
	class LuaEventReceiver : public ngn::UserEventReceiver
	{
	public:
		explicit LuaEventReceiver(const luaT::LuaRef& object);

		virtual int onKeyEvent( const ngn::KeyEvent& event, int argFromPreviousReceiver );

		virtual int onMouseEvent( const ngn::MouseEvent& event, int argFromPreviousReceiver );

	private:
		luaT::LuaRef lobject;
	};
}}

