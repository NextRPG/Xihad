#pragma once
#include "Engine/UpdateHandler.h"
#include "LuaT/lua_ref.h"

namespace xihad { namespace script
{
	class LuaUpdateHandler : public ngn::UpdateHandler
	{
	public:
		LuaUpdateHandler(luaT::LuaRef& table);

		luaT::LuaRef& getTable()
		{
			return mLuaObjectRef;
		}

	protected:
		virtual ~LuaUpdateHandler();

		virtual void onStart() override;

		virtual void onUpdate( const ngn::Timeline& ) override;

		virtual void onStop() override;

	protected:
		luaT::LuaRef mLuaObjectRef;
	};
}}


