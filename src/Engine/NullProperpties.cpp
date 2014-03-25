#include "NullProperpties.h"
#include "Lua\lua.hpp"

namespace xihad { namespace ngn
{
	void NullProperties::pushSelf( lua_State* L ) const 
	{
		lua_pushnil(L);
	}

}}

