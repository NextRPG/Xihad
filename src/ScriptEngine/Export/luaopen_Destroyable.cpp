#include "luaopen_Destroyable.h"
#include "Engine/Destroyable.h"
#include "LuaT/luaT.h"

using namespace luaT;
using namespace xihad::ngn;
luaT_defMetaData(Destroyable, true);

namespace xihad { namespace script
{
	int luaopen_Destroyable(lua_State* L)
	{
		luaT_defRegsBgn(regs)
			luaT_mnamedfunc(Destroyable, destroy),
		luaT_defRegsEnd
		MetatableFactory<Destroyable>::create(L, regs, 0);
		return 0;
	}
}}

