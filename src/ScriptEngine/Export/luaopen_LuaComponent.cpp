#include <luaT/luaT.h>
#include "../LuaComponent.h"

using namespace luaT;
using namespace xihad::ngn;
luaT_defMetaData(xihad::script::LuaComponent, false);

namespace xihad { namespace script
{
	int luaopen_LuaComponent( lua_State* L )
	{
		MetatableFactory<LuaComponent, Component>::createNull(L, 0);
		return 0;
	}

}}

