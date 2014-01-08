#pragma once
#include "Engine/Component.h"
#include "LuaT/lua_ref.h"
#include "LuaComponentSystemFactory.h"

namespace xihad { namespace script
{
	class LuaComponent : public ngn::Component
	{
	public:
		LuaComponent(const std::string& rttiName, ngn::GameObject& host, luaT::LuaRef& luaObject);

		luaT::LuaRef& getTable()
		{
			return mLuaObjectRef;
		}

	protected:
		virtual ~LuaComponent();

		virtual void onStart();

		virtual void onUpdate( const ngn::Timeline& time );

		virtual void onStop();

	public:
		DEFINE_VISITABLE

	private:
		luaT::LuaRef mLuaObjectRef;
	};
}}

