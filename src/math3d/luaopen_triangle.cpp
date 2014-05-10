#include <irrlicht/triangle3d.h>
#include "luaopen_util.h"

using namespace luaT;
using namespace irr::core;

namespace xihad { namespace script 
{
	int luaopen_triangle(lua_State* L)
	{
		luaT_defRegsBgn(triCtor)
			{ "triangle", luaT_constructor(triangle3df(const vector3df&, const vector3df&, const vector3df&)) },
		luaT_defRegsEnd
		luaL_register(L, "math3d", triCtor);

		luaT_defRegsBgn(triRegs)
			luaT_mnnamefunc(triangle3df, operator==, __eq),
			luaT_mnamedfunc(triangle3df, getNormal),
			luaT_mnamedfunc(triangle3df, getPlane),
			luaT_mnamedfunc(triangle3df, getArea),
		luaT_defRegsEnd
		MetatableFactory<triangle3df>::create(L, triRegs, 0);

		return 1;
	}
}}