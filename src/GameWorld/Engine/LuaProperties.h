#pragma once
#include "Properties.h"
#include "LuaT\lua_ref.h"

namespace xihad { namespace ngn
{
	/**
	 * 以 Lua table 表示的属性值集合。其中
	 */
	class LuaProperties : public Properties
	{
	public:
		explicit LuaProperties(const luaT::LuaRef& tableRef);	

		virtual void pushSelf(lua_State* L) const override;

		virtual B getBool  (S key, B pDefault = B()) const override;

		virtual I getInt   (S key, I pDefault = I()) const override;

		virtual F getFloat (S key, F pDefault = F()) const override;

		virtual D getDimen (S key, D pDefault = D()) const override;

		virtual V getVector(S key, V pDefault = V()) const override;

		virtual Q getQuater(S key, Q pDefault = Q()) const override;

		virtual S getString(S key, S pDefault = S()) const override;

	private:
		luaT::LuaRef lobject;
	};
}}

