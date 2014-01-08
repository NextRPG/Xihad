#include "metatable_factory.h"
#include "xassert.h"
#include "table_ops.h"
#include "reserved.h"

namespace luaT 
{
	void GeneralMetatableFactory::create( lua_State* L, const char* name, luaL_Reg* regs, lua_CFunction gc )
	{
		int success = luaL_newmetatable(L, name);
		xassert(success && "Metatable for this name has been used.");

		if (gc != NULL) setField(L, -1, "__gc", gc);

#ifdef _DEBUG
		lua_pushstring(L, name);
		lua_setfield(L, -2, ReservedKeyword::__TYPENAME);
#endif // _DEBUG

		// self.__index = self.
		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");

		// load self to package.
		if (regs != NULL) luaL_register(L, NULL, regs);	
	}


	void InheritenceResolver::link( lua_State* L, int offset, const char* mtName )
	{
		if (offset != 0)
		{
			lua_pushinteger(L, offset);
			lua_setfield(L, -2, ReservedKeyword::__OFFSET);	// offset to parent.
		}

		Metatable::byName(L, mtName);
		lua_setmetatable(L, -2);
	}

}
