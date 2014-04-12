#include <luaT/luaT.h>
#include <string>
#include "luaT/stack_memo.h"

using namespace luaT;
namespace xihad { namespace script
{
	static int createStdString(lua_State* L)
	{
		size_t size;
		const char* str = luaL_checklstring(L, 1, &size);
		void* mem = lua_newuserdata(L, sizeof(std::string));
		new (mem) std::string(str, size);
		Metatable::bind<std::string>(L, -1);

		return 1;
	}

	int luaopen_string( lua_State* L )
	{
		StackMemo memo(L);

		lua_getglobal(L, "_G");
		lua_pushcfunction(L, createStdString);
		lua_setfield(L, -2, "c");

		luaT_defRegsBgn(stringReg)
			luaT_mnamedfunc(std::string, c_str),
		luaT_defRegsEnd
		MetatableFactory<std::string>::create(L, stringReg, 0);
		
		return 0;
	}

}}

