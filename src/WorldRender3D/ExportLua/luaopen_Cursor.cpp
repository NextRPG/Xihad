#include <irrlicht/ICursorControl.h>
#include <Engine/position2d.h>
#include <luaT/luaT.h>

using namespace irr::gui;
using namespace luaT;

namespace xihad { namespace render3d
{
	static int getPosition(lua_State* L)
	{
		auto cursor = checkarg<ICursorControl*>(L, 1);
		const ngn::position2di& p = cursor->getPosition();
		lua_pushinteger(L, p.X);
		lua_pushinteger(L, p.Y);

		return 2;
	}

	int luaopen_Cursor(lua_State* L)
	{
		// cursor
		luaT_defRegsBgn(cursorRegs)
			luaT_mnamedfunc_ovl(ICursorControl, void (ICursorControl::*)(int, int), setPosition),
			luaT_lnamedfunc(getPosition),
			luaT_mnamedfunc(ICursorControl, setVisible),
			luaT_mnamedfunc(ICursorControl, isVisible),
		luaT_defRegsEnd
		MetatableFactory<ICursorControl>::create(L, cursorRegs);

		return 0;
	}
}}