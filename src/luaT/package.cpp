#include "package.h"
#include "cpp_adapter.h"

namespace luaT
{
	void Package::preload( lua_State* L, const char* name, lua_CFunction loadFunction )
	{
		luaT_defRegsBgn(regs)
			{ name, loadFunction },
		luaT_defRegsEnd

		Package::preload(L, regs);
	}

	void Package::preload( lua_State* L, luaL_Reg* libs )
	{
		lua_getglobal(L, "package");
		lua_getfield(L, -1, "preload");
		while (libs->name != NULL && libs->func != NULL)
		{
			lua_pushcfunction(L, libs->func);
			lua_setfield(L, -2, libs->name);
			++libs;
		}
		lua_pop(L, 2);
	}

}
