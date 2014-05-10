#include "LuaT\luaT.h"
#include "Engine\NativeWindow.h"

using namespace luaT;
using namespace xihad::ngn;
namespace xihad { namespace script
{
	static int getScreenSize(lua_State* L)
	{
		luaT_variable(L, 1, NativeWindow*, window);
		auto& size = window->getScreenSize();
		lua_pushinteger(L, size.Width);
		lua_pushinteger(L, size.Height);
		return 2;
	}

	int luaopen_NativeWindow( lua_State* L )
	{
		luaT_defRegsBgn(wnd)
			luaT_mnamedfunc(NativeWindow, isClosed),
			luaT_mnamedfunc(NativeWindow, isActive),
			luaT_mnamedfunc(NativeWindow, isFocused),
			luaT_mnamedfunc(NativeWindow, isFullScreen),
			luaT_mnamedfunc(NativeWindow, close),
			luaT_mnamedfunc(NativeWindow, setResizable),
			luaT_lnamedfunc(getScreenSize),
		luaT_defRegsEnd
		MetatableFactory<NativeWindow>::create(L, wnd);
		return 0;
	}
}}




