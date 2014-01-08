#include "new_userdata.h"
#include "metatable.h"

namespace luaT
{
	void* UserdataAllocator::allocate( lua_State* L, size_t size, const char* mtName )
	{
		void* mem = lua_newuserdata(L, size);
		Metatable::bind(L, -1, mtName);
		return mem;
	}

	void UserdataAllocator::allocate_for_ptr( lua_State* L, void* ptr, const char* mtName )
	{
		if (ptr == nullptr)
		{
			lua_pushnil(L);
		}
		else
		{
			lua_createtable(L, 0, 2);
			lua_pushlightuserdata(L, ptr);
			lua_setfield(L, -2, ReservedKeyword::__UDKEY); // set field __ud

			Metatable::bind(L, -1, mtName);

			lua_pushstring(L, "Not Accessable");
			lua_setfield(L, -2, "__metatable");	// protect metatable.
		}
	}

}

