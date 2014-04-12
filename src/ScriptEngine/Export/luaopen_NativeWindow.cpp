#include "LuaT\luaT.h"
#include "Engine\NativeWindow.h"

using namespace luaT;
using namespace xihad::ngn;
namespace xihad { namespace script
{
	int luaopen_NativeWindow( lua_State* L )
	{
		luaT_defRegsBgn(wnd)
			luaT_mnamedfunc(NativeWindow, isClosed),
			luaT_mnamedfunc(NativeWindow, isActive),
			luaT_mnamedfunc(NativeWindow, isFocused),
			luaT_mnamedfunc(NativeWindow, isFullScreen),
			luaT_mnamedfunc(NativeWindow, close),
			luaT_mnamedfunc(NativeWindow, setResizable),
		luaT_defRegsEnd
		MetatableFactory<NativeWindow>::create(L, wnd);
		return 0;
	}
}}




