#pragma once
#include "IStackPusher.h"
#include "luaT/luaT.h"

namespace xihad { namespace particle
{
	template <typename T>
	class CTplStackPusher : public IStackPusher
	{
	public:
		CTplStackPusher(T* v) : value(v) {}

		virtual void pushOnto(lua_State* L)
		{
			luaT::push<T*>(L, value);
		}

	private:
		T* value;
	};

}}

